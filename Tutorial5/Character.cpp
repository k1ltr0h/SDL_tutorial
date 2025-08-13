#include "Character.h"

Character::~Character(){}

Character::Character(SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_,
bool collidable_, bool grav_activated_):
GameObject::GameObject(sur_, rect_, pos_, collidable_, grav_activated_){
    set_object_type(GameObject::type::NPC);
}

void Character::update(float dt, Vector2D& camera_offset, const std::vector<GameObject*>& allObjects, bool centered){
    GameObject::update(dt, camera_offset, allObjects, centered);
    animate();
}

void Character::animate(){
    SDL_Rect src_rect = get_src_rect();
    int new_src_x = 0;
    bool need_flip = false;

    // Animación de sprites
    if(spriteFrameCounter % config.get_frames_per_sprite() == 0){
        // Si no se ha movido, no cambia el sprite
        if((get_move_dir_x() < 0 || get_move_dir_x() > 0) && get_surface_width() - src_rect.w > 0){
            new_src_x =  src_rect.w + (src_rect.x % (get_surface_width() - src_rect.w));
        }
        // Si se mueve a la izquierda, se voltea
        if(get_move_dir_x() < 0){
            if(!get_horizontal_flip()){
                need_flip = true;
            }
        }
        // Si se mueve a la derecha, no se voltea
        else if(get_move_dir_x() > 0){
            if(get_horizontal_flip()){
                need_flip = true;
            }
        }
        // Si requiere voltear, se voltea
        if(need_flip){
            //flip_surface();
            set_horizontal_flip(!get_horizontal_flip());
        }
        // Actualiza la posición del sprite en el sprite sheet
        set_src_rect(new_src_x, src_rect.y, src_rect.w, src_rect.h);
        spriteFrameCounter = 0;
    }
    // Incrementa el contador de frames
    spriteFrameCounter++;
}

void Character::jump(){
    //get_acceleration().set_y(-20000); Salto varía según valor de dt
    get_velocity().set_y(-700); // Salto no varía según valor de dt

    set_on_air(true);
}

void Character::attack(){

}

void Character::talk(){

}