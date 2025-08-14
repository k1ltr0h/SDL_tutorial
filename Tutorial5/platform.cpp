#include "Platform.h"

Platform::Platform(SDL_Surface* sprite, SDL_Rect rect_,
                   Vector2D pos_)
: GameObject(sprite, {0,0, sprite->w, sprite->h}, pos_, true, false)
{
    Hitbox hit_box = get_hit_box();
    Vector2D pos = get_position();
    
    set_object_type(GameObject::type::PLATFORM);
    set_static(true);

    total_width_ = rect_.w;
    total_height_ = rect_.h;

    tile_width_ = sprite->w;
    tile_height_ = sprite->h;

    const float center_x_full = pos.get_x() + (total_width_ * 0.5f);
    hit_box.width  = total_width_;
    hit_box.height = total_height_;
    hit_box.center.set_x(center_x_full);

    set_hit_box(hit_box);
}

void Platform::render(SDL_Renderer* renderer){
    // Guardar estado de rects para restaurar al final
    SDL_Rect src_rect = get_src_rect();
    SDL_Rect dst_rect = get_dst_rect();

    // 1) Dibuja el primer tile utilizando la lógica existente de GameObject
    GameObject::render(renderer);

    // Si la textura aún no existía, GameObject::render() la habrá creado.
    SDL_Texture* tex = get_texture();
    if (!tex) {
        return;
    }

    // 2) Dibujar los tiles restantes (del 2º al último)
    const int tile_w = src_rect.w;
    //const int tile_h = src_rect.h;

    // Nada que hacer si el ancho total cabe en un solo tile
    if (total_width_ <= tile_w) {
        return;
    }

    // Reutilizamos un src local para NO tocar el rect original (evita parpadeos o efectos no deseados)
    SDL_Rect src_part = src_rect;
    SDL_Rect dst_tile = dst_rect;

    // Empezamos desde tile_w porque el primer tile ya se dibujó
    for (int offset_x = tile_w; offset_x < total_width_; offset_x += tile_w) {
        const int remaining = total_width_ - offset_x;
        dst_tile.x = dst_rect.x + offset_x;

        // Último segmento puede quedar más corto que tile_w
        if (remaining < tile_w) {
            dst_tile.w = remaining;
            src_part.w = remaining;
        } else {
            dst_tile.w = tile_w;
            src_part.w = tile_w;
        }

        // Copiamos directamente (no necesitamos flip ni rotación para las losas extra)
        SDL_RenderCopy(renderer, tex, &src_part, &dst_tile);
    }
}

void Platform::update(float dt,
                      Vector2D& camera_offset,
                      const std::vector<GameObject*>& all_objects,
                      bool centered){

    Vector2D pos = get_position();
    SDL_Rect dst_rect = get_dst_rect();
    // 1) Actualizar dst_rect según la posición y el offset de la cámara
    // No es necesario centrar el tile porque la plataforma no está centrada
    // solo se ajusta verticalmente. Su x e y está en la esquina inferior izquierda.
    set_dst_rect_position(int(pos.get_x() - camera_offset.get_x()),
                          int(pos.get_y() - dst_rect.h - camera_offset.get_y()));
}
