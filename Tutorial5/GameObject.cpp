#include "GameObject.h"

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
        acc = Vector2D(0, config.get_gravity()); // gravedad por defecto
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

void GameObject::resolve_collisions(float dt, const std::vector<GameObject*>& all_objects){
    // 1) Predicción completa
    Vector2D pos_next = predict_position(dt);
    Vector2D vel_next = predict_velocity(dt);

    // 2) Desplazamiento del frame
    float dx = pos_next.get_x() - pos.get_x();
    float dy = pos_next.get_y() - pos.get_y();

    // 3) Broad-phase (usar centros de hitbox)
    Hitbox a = get_hit_box();                // centro actual
    float current_center_x = a.center.get_x();
    float current_center_y = a.center.get_y();

    float next_center_x = pos_next.get_x();
    float next_center_y = pos_next.get_y() - dst_rect.h * 0.5f;

    // calcular los límites de búsqueda
    float min_x = std::min(current_center_x, next_center_x) - a.width  * 0.5f;
    float max_x = std::max(current_center_x, next_center_x) + a.width  * 0.5f;
    float min_y = std::min(current_center_y, next_center_y) - a.height * 0.5f;
    float max_y = std::max(current_center_y, next_center_y) + a.height * 0.5f;

    float impact_fraction = 1.0f;
    int impact_axis = -1;   // 0=X, 1=Y
    const GameObject* impact_target = nullptr;

    for (const GameObject* other : all_objects) {
        if (other == this || !other->is_collidable()) continue;

        Hitbox b = other->get_hit_box();
        // Broad-phase: descartar rápido
        if (b.right()  < min_x || b.left() > max_x ||
            b.bottom() < min_y || b.top()  > max_y)
            continue;

        float impact_fraction_other; 
        int impact_axis_other;
        // 4) Narrow-phase: swept AABB
        if (swept_aabb(*other, dx, dy, impact_fraction_other, impact_axis_other) && impact_fraction_other < impact_fraction) {
            impact_fraction = impact_fraction_other;
            impact_axis     = impact_axis_other;
            impact_target   = other;
        }
    }

    // 5) Recalcular pocisión
    //    Ahora tenemos el objeto de impacto y el tiempo más cercano de colisión
    if (impact_target) {
        const float EPS = config.get_eps(); // pega 1px por fuera

        // Calcular el tiempo de impacto y el tiempo restante
        float time_to_collision = impact_fraction * dt;
        float remaining_t = dt - time_to_collision;

        // Avanza hasta el instante exacto del impacto
        update_position(time_to_collision);
        update_velocity(time_to_collision);
        // Releer hitboxes con pos en el tiempo de impacto
        Hitbox a_hitbox = get_hit_box();
        Hitbox b_hitbox = impact_target->get_hit_box();

        const float vel_x = vel.get_x();
        const float vel_y = vel.get_y();
        const float acc_x = acc.get_x();
        const float acc_y = acc.get_y();

        // Snap y bloqueo de eje
        if (impact_axis == 0) {
            if (dx > 0.0f) {
                // venía de izquierda → encaja a la izquierda del obstáculo
                pos.set_x(b_hitbox.left() - a_hitbox.width / 2.0f - EPS);
                
            } else {
                // venía de derecha → encaja a la derecha del obstáculo
                pos.set_x(b_hitbox.right() + a_hitbox.width / 2.0f + EPS);
            }
            // Consume remainingT solo en Y
            printf("Collision on X axis: pos.x = %f\n", pos.get_x());
            vel.set_x(0.0f);
            printf("Velocidad X: %.2f\n Acc X: %.2f\n", vel_x, acc_x);
            pos.set_y(pos.get_y() + vel_y * remaining_t + 0.5f * acc_y * remaining_t * remaining_t);
            vel.set_y(vel_y + acc_y * remaining_t);
        } else {
            // OJO: pos.y es bottom-center del jugador
            if (dy > 0.0f) {
                // Caía desde arriba: deja el bottom justo encima del top del obstáculo
                pos.set_y(b_hitbox.top() - EPS);
                set_on_air(false);
                // anula solo el movimiento vertical restante
                vel.set_y(0.0f);
                acc.set_y(0.0f);         // evita re-penetrar en el remDT
                // consume remDT solo en X
                pos.set_x(pos.get_x() + vel_x * remaining_t + 0.5f * acc_x * remaining_t * remaining_t);
                vel.set_x(vel_x + acc_x * remaining_t);
            } else {
                // Venía desde abajo: deja el top justo debajo del bottom del obstáculo
                // top del jugador = pos.y - Ah.height  ⇒ pos.y = Bh.bottom() + Ah.height + EPS
                pos.set_y(b_hitbox.bottom() + a_hitbox.height + EPS);
                // anula solo el vertical
                vel.set_y(0.0f);
                acc.set_y(0.0f);
                // consume remDT solo en X
                pos.set_x(pos.get_x() + vel_x * remaining_t + 0.5f * acc_x * remaining_t * remaining_t);
                vel.set_x(vel_x + acc_x * remaining_t);
            }
        }
    } else {
        // Sin colisión: aplica predicción completa
        pos = pos_next;
        vel = vel_next;
    }
}

bool GameObject::swept_aabb(const GameObject& obstacle,
                            float dx, float dy,
                            float& impact_fraction_other, int& impact_axis_other) const{
    const Hitbox& a = this->get_hit_box();
    const Hitbox& b = obstacle.get_hit_box();

    float tx_entry, tx_exit;
    float ty_entry, ty_exit;

    if (dx > 0.0f) {
        tx_entry = (b.left() - a.right()) / dx;
        tx_exit  = (b.right() - a.left()) / dx;
    } else if (dx < 0.0f) {
        tx_entry = (b.right() - a.left()) / dx;
        tx_exit  = (b.left() - a.right()) / dx;
    } else {
        tx_entry = -std::numeric_limits<float>::infinity();
        tx_exit  =  std::numeric_limits<float>::infinity();
    }

    if (dy > 0.0f) {
        ty_entry = (b.top() - a.bottom()) / dy;
        ty_exit  = (b.bottom() - a.top()) / dy;
    } else if (dy < 0.0f) {
        ty_entry = (b.bottom() - a.top()) / dy;
        ty_exit  = (b.top() - a.bottom()) / dy;
    } else {
        ty_entry = -std::numeric_limits<float>::infinity();
        ty_exit  =  std::numeric_limits<float>::infinity();
    }

    float t_entry = std::max(tx_entry, ty_entry);
    float t_exit  = std::min(tx_exit, ty_exit);

    // Comprueba que el tiempo de entrada sea válido
    // y que no haya salida antes de entrar
    if (t_entry < 0.0f || t_entry > 1.0f || t_entry > t_exit)
        return false;

    impact_fraction_other = t_entry;
    // t_entry mayor representa el eje de colisión
    // 0 = X, 1 = Y
    impact_axis_other = (tx_entry > ty_entry ? 0 : 1);

    return true;
}

void GameObject::update(float dt,
                        const std::vector<GameObject*>& all_objects,
                        bool centered){
    resolve_collisions(dt, all_objects);

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
        case dir::RIGHT: 
            acc.set_x(+MOVE_ACCEL);
            move_dir_x = 1;
            break;
        case dir::LEFT:  
            acc.set_x(-MOVE_ACCEL); 
            move_dir_x = -1;
            break;
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

    float vel_x = vel.get_x();

    if (std::abs(vel_x) > BRAKE_THRESHOLD) {
        // Aplica aceleración contraria al movimiento
        acc.set_x(vel_x > 0.0f ? -BRAKE_ACCEL : BRAKE_ACCEL);
    } else {
        // Velocidad suficientemente baja → parar
        vel.set_x(0.0f);
        acc.set_x(0.0f);
    }

    // Gravedad vertical siempre (o cero si no activada)
    acc.set_y(gravity_activated ? config.get_gravity() : 0.0f);
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