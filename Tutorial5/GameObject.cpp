#include "GameObject.h"

GameObject::~GameObject(){
    if (texture) SDL_DestroyTexture(texture);
}

GameObject::GameObject(SDL_Surface* sur_, 
                       SDL_Rect rect_, 
                       Vector2D pos_, 
                       bool collidable_, 
                       bool grav_activated_){
    (sur_ != NULL)? surface_width = sur_->w : surface_width = rect_.w;
    (sur_ != NULL)? surface_height = sur_->h : surface_height = rect_.h;
    surface = sur_;

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

    hit_box = {{pos.get_x(), 
                pos.get_y() - rect_.h /2},
                rect_.w, 
                rect_.h};

    src_rect = rect_;
    dst_rect = {int(pos.get_x()), int(pos.get_y()), rect_.w, rect_.h};
}

void GameObject::render(SDL_Renderer* renderer){
    if (!texture) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_Log("Error creando streaming-texture: %s", SDL_GetError());
            return;
        }
        // Si 0 < alpha < 255 mezcla (blend) el fondo con la textura
        //SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); 

        SDL_FreeSurface(surface);
    }

    SDL_RenderCopyEx(renderer, this->texture, &src_rect, &dst_rect, 0.0, nullptr, 
                     get_horizontal_flip() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void GameObject::resolve_collisions(float dt, const std::vector<GameObject*>& all_objects){
    // Límite de seguridad para evitar loops infinitos si quedara "encajado"
    std::vector<const GameObject*> candidates;
    const int max_impacts_per_frame = 4;
    float remaining_t = dt;
    int impacts = 0;

    Vector2D pos_next = predict_position(remaining_t);
    Vector2D vel_next = predict_velocity(remaining_t);

    // 2) Broad‑phase con estado ACTUAL (no del frame completo)
    Hitbox a = get_hit_box();
    float current_center_x = a.center.get_x();
    float current_center_y = a.center.get_y();

    float next_center_x = pos_next.get_x();
    float next_center_y = pos_next.get_y() - dst_rect.h * 0.5f;

    // Area de búsqueda para colisiones
    float min_x = std::min(current_center_x, next_center_x) - a.width  * 0.5f;
    float max_x = std::max(current_center_x, next_center_x) + a.width  * 0.5f;
    float min_y = std::min(current_center_y, next_center_y) - a.height * 0.5f;
    float max_y = std::max(current_center_y, next_center_y) + a.height * 0.5f;

    for (const GameObject* other : all_objects) {
        if (other == this || !(other->is_collidable()))
            continue;

        const Hitbox b = other->get_hit_box();

        // Broad‑phase rápido
        if (b.right()  < min_x || b.left() > max_x ||
            b.bottom() < min_y || b.top()  > max_y){
                // Si no hay colisión, saltar al siguiente objeto

                continue;
            }
        // Si hay colisión, añadir a la lista de candidatos
        // para el cálculo de colisiones continuas
        candidates.push_back(other);
    }

    set_on_air(true); // Si no hay colisión con el suelo, se considera que está en el aire

    // Bucle: consumir dt en varios “sub‑pasos” si hay impactos encadenados
    while (remaining_t > 0.0f && impacts < max_impacts_per_frame) {
        // 1) Predicción con el tiempo restante
        pos_next = predict_position(remaining_t);
        vel_next = predict_velocity(remaining_t);

        float dx = pos_next.get_x() - pos.get_x();
        float dy = pos_next.get_y() - pos.get_y();

        // 2) Broad‑phase con estado ACTUAL (no del frame completo)
        a = get_hit_box();
        current_center_x = a.center.get_x();
        current_center_y = a.center.get_y();

        next_center_x = pos_next.get_x();
        next_center_y = pos_next.get_y() - dst_rect.h * 0.5f;

        // Area de búsqueda para colisiones
        min_x = std::min(current_center_x, next_center_x) - a.width  * 0.5f;
        max_x = std::max(current_center_x, next_center_x) + a.width  * 0.5f;
        min_y = std::min(current_center_y, next_center_y) - a.height * 0.5f;
        max_y = std::max(current_center_y, next_center_y) + a.height * 0.5f;

        float impact_fraction = 1.0f;   // en [0..1] respecto a remaining_t
        int impact_axis = -1;           // 0 = X, 1 = Y
        const GameObject* impact_target = nullptr;

        for (const GameObject* other : candidates) {
            if (other == this || !(other->is_collidable()))
                continue;

            const Hitbox b = other->get_hit_box();

            // Broad‑phase rápido
            if (b.right()  < min_x || b.left() > max_x ||
                b.bottom() < min_y || b.top()  > max_y){
                    // Si no hay colisión, saltar al siguiente objeto
                    std::swap(other, candidates.back()); 
                    candidates.pop_back();    
                    continue;
                }

            float candidate_t; 
            int candidate_axis;
            if (swept_aabb(*other, dx, dy, candidate_t, candidate_axis) && candidate_t < impact_fraction) {
                impact_fraction = candidate_t;
                impact_axis     = candidate_axis;
                impact_target   = other;
            }
        }

        if (!impact_target) {
            // 3a) No hay impactos: avanzar todo el tiempo restante y salir
            update_position(remaining_t);
            update_velocity(remaining_t);
            remaining_t = 0.0f;
            break;
        }

        // 3b) Hay impacto: avanzar SOLO hasta el impacto
        const float eps = config.get_eps(); // p. ej. 0.5f para evitar sticking
        float time_to_collision = impact_fraction * remaining_t;

        // Avanzar justo hasta el momento del impacto
        update_position(time_to_collision);
        update_velocity(time_to_collision);

        // Releer AABB de ambos en el instante del choque
        Hitbox a_hitbox = get_hit_box();
        Hitbox b_hitbox = impact_target->get_hit_box();

        // “Snap” al borde del obstáculo y bloquear eje golpeado
        if (impact_axis == 0) {
            // Choque en X
            if (dx > 0.0f) {
                pos.set_x(b_hitbox.left() - a_hitbox.width * 0.5f - eps);
            } else {
                pos.set_x(b_hitbox.right() + a_hitbox.width * 0.5f + eps);
            }
            stop(axis::ABSCISSA); // anula movimiento horizontal restante
        } else {
            // Choque en Y (recuerda: pos.y = bottom‑center del objeto) 
            if (dy > 0.0f) {
                // Caía desde arriba → apoyar sobre el top del obstáculo
                pos.set_y(b_hitbox.top() - eps);
                set_on_air(false);
                set_jumped_since_grounded(false);
            } else {
                // Venía desde abajo → pegar bajo el obstáculo
                pos.set_y(b_hitbox.bottom() + a_hitbox.height + eps);
            }
            stop(axis::ORDINATE); // anula movimiento vertical restante
        }

        // 4) Consumir el tiempo del impacto y repetir para el tiempo restante
        remaining_t -= time_to_collision;
        impacts++;
        // Nota: NO avanzamos aquí “el resto” en el eje libre. Volvemos al inicio del while,
        //       recalculamos broad‑phase y, si no hay nuevos impactos, avanzaremos el
        //       movimiento restante en el branch “sin colisión”. Así la segunda colisión
        //       dentro del mismo frame SÍ se detecta.
    }

    // Si excedimos el máximo de impactos, evita deadlocks consumiendo el tiempo que quede
    if (remaining_t > 0.0f && impacts >= max_impacts_per_frame) {
        update_position(remaining_t);
        update_velocity(remaining_t);
    }
}

bool GameObject::swept_aabb(const GameObject& obstacle,
                            float dx, float dy,
                            float& impact_fraction, int& impact_axis) const
{
    const Hitbox& a = this->get_hit_box();
    const Hitbox& b = obstacle.get_hit_box();

    // Origen del rayo = centro de A
    const float center_x = a.center.get_x();
    const float center_y = a.center.get_y();

    // AABB expandido = B ± (halfA + halfB)
    const float half_sum_w = 0.5f * (a.width  + b.width);
    const float half_sum_h = 0.5f * (a.height + b.height);

    // Definir los límites de B
    // B = (bx_min, by_min) a (bx_max, by_max)
    const float bx_min = b.center.get_x() - half_sum_w;
    const float bx_max = b.center.get_x() + half_sum_w;
    const float by_min = b.center.get_y() - half_sum_h;
    const float by_max = b.center.get_y() + half_sum_h;

    // Función labmda para evitar división por cero
    // y devolver infinito si el denominador es cero
    float (*safe_inverse)(float) = [](float v) -> float {
        const float eps = 1e-8f;
        return (std::fabs(v) < eps) ? std::numeric_limits<float>::infinity() : 1.0f / v;
    };

    const float inv_dx = safe_inverse(dx);
    const float inv_dy = safe_inverse(dy);

    // Calcular tiempos de entrada y salida en X e Y
    float tx_entry = (bx_min - center_x) * inv_dx;
    float tx_exit = (bx_max - center_x) * inv_dx;
    float ty_entry = (by_min - center_y) * inv_dy;
    float ty_exit = (by_max - center_y) * inv_dy;

    // Asegurar que tx_entry < tx_exit y ty_entry < ty_exit
    float tmin_x = std::min(tx_entry, tx_exit);
    float tmax_x = std::max(tx_entry, tx_exit);
    float tmin_y = std::min(ty_entry, ty_exit);
    float tmax_y = std::max(ty_entry, ty_exit);

    // Tiempos de entrada y salida de colisión
    float t_entry = std::max(tmin_x, tmin_y);
    float t_exit  = std::min(tmax_x, tmax_y);

    // Comprueba que el tiempo de entrada sea válido
    // y que no haya salida antes de entrar
    if (t_entry < 0.0f || t_entry > 1.0f || t_entry > t_exit)
        return false;

    impact_fraction = t_entry;
    // Eje del impacto según qué “slab” gobierna la entrada
    impact_axis = (tmin_x > tmin_y) ? 0 : 1; // 0 = X, 1 = Y

    return true;
}

void GameObject::update(float dt,
                        Vector2D& camera_offset,
                        const std::vector<GameObject*>& all_objects,
                        bool centered){

    if (!static_object) {
        resolve_collisions(dt, all_objects);
        brake();

        hit_box.center = { pos.get_x(), pos.get_y() - dst_rect.h * 0.5f };
    }

    update_dst_rect_from_pos(camera_offset, centered);
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

Vector2D GameObject::predict_position(float dt) const{
    // Predice la nueva posición basándose en la velocidad y aceleración
    // dt: tiempo transcurrido desde el último frame (en segundos)
    float x = pos.get_x() + vel.get_x()*dt + (acc.get_x()/2) * (dt*dt);
    float y = pos.get_y() + vel.get_y()*dt + (acc.get_y()/2) * (dt*dt);

    return Vector2D(x, y);
}

Vector2D GameObject::predict_velocity(float dt) const{
    // Predice la nueva velocidad basándose en la aceleración
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