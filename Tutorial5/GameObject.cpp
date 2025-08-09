#include "GameObject.h"

#define G 2000 ///< Aceleración de Gravedad: positivo porque 'y' crece hacia abajo en la pantalla.
#define GROUND 450

GameObject::~GameObject(){
    if (texture) SDL_DestroyTexture(texture);
    if (surface) SDL_FreeSurface(surface);
}

GameObject::GameObject(SDL_Surface* sur_, 
                       SDL_Rect rect_, 
                       Vector2D pos_, 
                       bool collidable_, 
                       bool grav_activated_){
    (sur_ != NULL)? surface_width = sur_->w : surface_width = rect_.w;
    (sur_ != NULL)? surface_height = sur_->h : surface_height = rect_.h;
    surface = sur_;
    //vertical_flip_surface = flip_surface(axis::ORDINATE);
    //horizontal_flip_surface = flip_surface(axis::ABSCISSA);
    object_type = type::PLATFORM;
    depth = 10;
    pos = pos_;
    vel = Vector2D();
    if(grav_activated_){
        acc = Vector2D(0, G);
    }
    else{
        acc = Vector2D(0, 0);
    }
    max_vel_x = 300, max_vel_y = 10000;

    gravity_activated = grav_activated_;
    collidable = collidable_;

    hit_box = {{pos.get_x() - rect_.w /2, 
                pos.get_y() - rect_.h /2},
                rect_.w, 
                rect_.h};

    src_rect = rect_;
    dst_rect = {int(pos.get_x()), int(pos.get_y()), rect_.w, rect_.h};
}

void GameObject::render(SDL_Renderer* renderer){
    if (!texture) {
        texture = SDL_CreateTexture(
            renderer,
            surface->format->format,
            SDL_TEXTUREACCESS_STREAMING,
            surface->w,
            surface->h
        );
        if (!texture) {
            SDL_Log("Error creando streaming-texture: %s", SDL_GetError());
            return;
        }
    }

    SDL_UpdateTexture(
       texture,
        nullptr,                  // actualiza toda la textura
        surface->pixels,
        surface->pitch
    );

    SDL_RenderCopy(renderer, this->texture, &src_rect, &dst_rect);
}

void GameObject::resolve_collisions(float dt, const std::vector<GameObject*>& allObjects){
    // 1) Predicción completa
    Vector2D posNext = predict_position(dt);
    Vector2D velNext = predict_velocity(dt);

    // 2) Desplazamiento del frame
    float dx = posNext.get_x() - pos.get_x();
    float dy = posNext.get_y() - pos.get_y();

    // 3) Broad-phase (usar centros de hitbox)
    Hitbox A = get_hit_box();                // centro actual
    float currentCenterX = A.center.get_x();
    float currentCenterY = A.center.get_y();

    float nextCenterX = posNext.get_x();
    float nextCenterY = posNext.get_y() - dst_rect.h * 0.5f;

    // calcular los límites de búsqueda
    float minX = std::min(currentCenterX, nextCenterX) - A.width  * 0.5f;
    float maxX = std::max(currentCenterX, nextCenterX) + A.width  * 0.5f;
    float minY = std::min(currentCenterY, nextCenterY) - A.height * 0.5f;
    float maxY = std::max(currentCenterY, nextCenterY) + A.height * 0.5f;

    float impactTime = 1.0f;
    int   hitAxis     = -1;   // 0=X, 1=Y
    const GameObject* hitTarget = nullptr;

    for (const GameObject* other : allObjects) {
        if (other == this || !other->is_collidable()) continue;

        Hitbox B = other->get_hit_box();
        // Broad-phase: descartar rápido
        if (B.right()  < minX || B.left() > maxX ||
            B.bottom() < minY || B.top()  > maxY)
            continue;

        float impactTimeOther; 
        int impactAxisOther;
        // 4) Narrow-phase: swept AABB
        if (swept_aabb(*other, dx, dy, impactTimeOther, impactAxisOther) && impactTimeOther < impactTime) {
            impactTime = impactTimeOther;
            hitAxis     = impactAxisOther;
            hitTarget   = other;
        }
    }

    // 5) Resolver
    if (hitTarget) {
        const float EPS = 1.0f; // pega 1px por fuera

        float timeToCollision = impactTime * dt;
        float remDT = dt - timeToCollision;

        // Avanza hasta el instante exacto del impacto
        pos = predict_position(timeToCollision);
        // Releer hitboxes con pos en el tiempo de impacto
        Hitbox Ah = get_hit_box();
        Hitbox Bh = hitTarget->get_hit_box();

        // Snap y bloqueo de eje
        if (hitAxis == 0) {
            const float EPS = 1.0f;
            Hitbox Ah = get_hit_box();
            Hitbox Bh = hitTarget->get_hit_box();

            if (dx > 0.0f) {
                // venía de izquierda → encaja a la izquierda del obstáculo
                pos.set_x(Bh.left() - Ah.width / 2.0f - EPS);
            } else {
                // venía de derecha → encaja a la derecha del obstáculo
                pos.set_x(Bh.right() + Ah.width / 2.0f + EPS);
            }
            // anula X y consume remDT solo en Y
            const float ay = acc.get_y();
            const float vy = vel.get_y();
            vel.set_x(0.0f);
            acc.set_x(0.0f);
            pos.set_y(pos.get_y() + vy * remDT + 0.5f * ay * remDT * remDT);
            vel.set_y(vy + ay * remDT);
        } else {
            // Tiempo hasta colisión y tiempo restante ya calculados
            // pos = predict_position(timeToCollision);  // ya hecho antes
            Hitbox Ah = get_hit_box();
            Hitbox Bh = hitTarget->get_hit_box();

            // OJO: pos.y es bottom-center del jugador
            if (dy > 0.0f) {
                // Caía desde arriba: deja el bottom justo encima del top del obstáculo
                pos.set_y(Bh.top() - EPS);
                set_on_air(false);
                // anula solo el movimiento vertical restante
                vel.set_y(0.0f);
                acc.set_y(0.0f);         // evita re-penetrar en el remDT
                // consume remDT solo en X
                const float ax = acc.get_x();
                const float vx = vel.get_x();
                pos.set_x(pos.get_x() + vx * remDT + 0.5f * ax * remDT * remDT);
                vel.set_x(vx + ax * remDT);
            } else {
                // Venía desde abajo: deja el top justo debajo del bottom del obstáculo
                // top del jugador = pos.y - Ah.height  ⇒ pos.y = Bh.bottom() + Ah.height + EPS
                pos.set_y(Bh.bottom() + Ah.height + EPS);
                // anula solo el vertical
                vel.set_y(0.0f);
                acc.set_y(0.0f);
                // consume remDT solo en X
                const float ax = acc.get_x();
                const float vx = vel.get_x();
                pos.set_x(pos.get_x() + vx * remDT + 0.5f * ax * remDT * remDT);
                vel.set_x(vx + ax * remDT);
            }
        }
    } else {
        // Sin colisión: aplica predicción completa
        pos = posNext;
        vel = velNext;
    }
}

bool GameObject::swept_aabb(const GameObject& obstacle,
                            float dx, float dy,
                            float& outT, int& outAxis) const{
    const Hitbox& A = this->get_hit_box();
    const Hitbox& B = obstacle.get_hit_box();

    float txEntry, txExit;
    float tyEntry, tyExit;

    if (dx > 0.0f) {
        txEntry = (B.left() - A.right()) / dx;
        txExit  = (B.right() - A.left()) / dx;
    } else if (dx < 0.0f) {
        txEntry = (B.right() - A.left()) / dx;
        txExit  = (B.left() - A.right()) / dx;
    } else {
        txEntry = -std::numeric_limits<float>::infinity();
        txExit  =  std::numeric_limits<float>::infinity();
    }

    if (dy > 0.0f) {
        tyEntry = (B.top() - A.bottom()) / dy;
        tyExit  = (B.bottom() - A.top()) / dy;
    } else if (dy < 0.0f) {
        tyEntry = (B.bottom() - A.top()) / dy;
        tyExit  = (B.top() - A.bottom()) / dy;
    } else {
        tyEntry = -std::numeric_limits<float>::infinity();
        tyExit  =  std::numeric_limits<float>::infinity();
    }

    float tEntry = std::max(txEntry, tyEntry);
    float tExit  = std::min(txExit, tyExit);

    // Comprueba que se esté dentro del objeto.
    if (tEntry < 0.0f || tEntry > 1.0f || tEntry > tExit)
        return false;

    outT = tEntry;
    outAxis = (txEntry > tyEntry ? 0 : 1);

    return true;
}

void GameObject::update(float dt,
                        const std::vector<GameObject*>& allObjects,
                        bool centered){
    resolve_collisions(dt, allObjects);

    // 6) Resto de física “global”
    ground();
    brake();

    hit_box.center = { pos.get_x(), pos.get_y() - dst_rect.h * 0.5f };
    hit_box.width  = dst_rect.w;
    hit_box.height = dst_rect.h;

    // 7) Actualiza dst_rect / hitbox desde pos (pos = bottom-center)
    if (centered) {
        center();
    } else {
        dst_rect = {
            int(pos.get_x() - dst_rect.w/2),
            int(pos.get_y() - dst_rect.h),
            dst_rect.w,
            dst_rect.h
        };
    }
}


void GameObject::move(int dir){
    // Aceleración de usuario, en px/s²
    constexpr float MOVE_ACCEL = 500.0f;

    switch(dir){
        case dir::RIGHT: acc.set_x(+MOVE_ACCEL); break;
        case dir::LEFT:  acc.set_x(-MOVE_ACCEL); break;
        case dir::UP:    acc.set_y(-MOVE_ACCEL); break;
        case dir::DOWN:  acc.set_y(+MOVE_ACCEL); break;
        default: break;
    }
}

void GameObject::stop(axis axis){
    if(axis == axis::ORDINATE){
        vel.set_y(0);
        acc.set_y(0);
    }
    else if(axis == axis::ABSCISSA){
        vel.set_x(0);
        acc.set_x(0);
    }
}

void GameObject::brake() {
    // Configuración de freno (px/s²) y umbral (px/s)
    const float BRAKE_ACCEL     = 500.0f;  ///< desaceleración por fricción
    const float BRAKE_THRESHOLD = 10.0f;   ///< si |vx| < umbral, consideramos detenido

    float vx = vel.get_x();

    if (std::abs(vx) > BRAKE_THRESHOLD) {
        // Aplica aceleración contraria al movimiento
        acc.set_x(vx > 0.0f ? -BRAKE_ACCEL : BRAKE_ACCEL);
    } else {
        // Velocidad suficientemente baja → parar
        vel.set_x(0.0f);
        acc.set_x(0.0f);
    }

    // Gravedad vertical siempre (o cero si no activada)
    acc.set_y(gravity_activated ? G : 0.0f);
}

void GameObject::ground(){ // Hard-coding for fake ground
    //vel_y = get_velocity();
    //pos_y = get_position();
    
    if(pos.get_y() >= GROUND && vel.get_y() > 0){
        //vel_y = 0;
        pos.set_y(GROUND);
        vel.set_y(0);
        on_air = false;
    }
}

void GameObject::center(){
    int half_width = src_rect.w/2;
    int half_height = src_rect.h/2;
    int all_height = src_rect.h;
    
    dst_rect = {int(pos.get_x() - half_width), int(pos.get_y() - all_height), dst_rect.w, dst_rect.h};
}

void GameObject::blit_surface(GameObject* obj){
    SDL_Rect obj_src_rect = obj->get_src_rect();
    SDL_Rect obj_dst_rect = obj->get_dst_rect();
    // Recorta imagen desde sprite set y lo dibuja en rectangulo definido en Last_Surface 
    SDL_BlitSurface(obj->get_surface(), &obj_src_rect, surface, &obj_dst_rect);
}

void GameObject::flip_surface(axis axis){
    SDL_LockSurface(surface);

    int pitch = surface->pitch; // row size
    char* temp = new char[pitch]; // intermediate buffer
    char* pixels = (char*) surface->pixels;

    if(axis == axis::ABSCISSA){
       for(int i=0; i < surface->h; ++i){
            // get pointers to the two rows to swap
            char* row = pixels + i * pitch;
            memcpy(temp, row, pitch);
            // RGB to BGR
            for(int iter=0; iter < pitch; iter+=3){
                pixels[iter + i*pitch] = temp[pitch - iter - 3];
                pixels[iter + 1 + i*pitch] = temp[pitch - iter - 2];
                pixels[iter + 2 + i*pitch] = temp[pitch - iter - 1];
            }
        } 
    }
    
    else if(axis == axis::ORDINATE){
        for(int i = 0; i < surface->h / 2; ++i) {
            // get pointers to the two rows to swap
            char* row_above = pixels + i * pitch;
            char* row_below = pixels + (surface->h - i - 1) * pitch;
            
            // swap rows
            memcpy(temp, row_above, pitch);
            memcpy(row_above, row_below, pitch);
            memcpy(row_below, temp, pitch);
        }
    }

    delete[] temp;

    SDL_UnlockSurface(surface);
}



Vector2D GameObject::predict_position(float dt) const{
    float x = pos.get_x() + vel.get_x()*dt + (acc.get_x()/2) * (dt*dt);
    float y = pos.get_y() + vel.get_y()*dt + (acc.get_y()/2) * (dt*dt);

    return Vector2D(x, y);
}

Vector2D GameObject::predict_velocity(float dt) const{
    float x = vel.get_x() + acc.get_x()*dt;
    float y = vel.get_y() + acc.get_y()*dt;

    if(x > max_vel_x){
        x = max_vel_x;
    }
    else if(x < -max_vel_x){
        x = -max_vel_x;
    }

    if(y > max_vel_y){
        y = max_vel_y;
    }
    else if(y < -max_vel_y){
        y = -max_vel_y;
    }
    
    return Vector2D(x, y);
}

void GameObject::update_velocity(float dt) {
    vel = predict_velocity(dt);
}

void GameObject::update_position(float dt) {
    pos = predict_position(dt);
}