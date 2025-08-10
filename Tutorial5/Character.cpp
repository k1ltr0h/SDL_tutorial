#include "Character.h"

Character::~Character(){}

Character::Character(SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_,
bool collidable_, bool grav_activated_):
GameObject::GameObject(sur_, rect_, pos_, collidable_, grav_activated_){
    set_object_type(GameObject::type::NPC);
}

void Character::update(float dt, const std::vector<GameObject*>& allObjects, bool centered){
    GameObject::update(dt, allObjects, centered);

    SDL_Rect src_rect = get_src_rect();
    Vector2D vel =  get_velocity();
    int new_src_x = 0;
    bool need_flip = false;

    // Draw sprite movement
    if(spriteFrameCounter % config.get_framesPerSprite() == 0){
        if(get_move_dir_x() < 0){ // make the flip
            // Si la velocidad es negativa, significa que se mueve a la izquierda
            // y por tanto el sprite debe estar volteado horizontalmente
            // eso significa que el sprite debe empezar desde el final de la imagen
            // y avanzar hacia la izquierda sin contar el primer sprite que representa el idle.
            new_src_x = (src_rect.x + src_rect.w) % (get_surface_width() - src_rect.w);
            // Si el sprite está volteado horizontalmente, no es necesario volver a voltear
            if(!get_horizontal_flip()){
                need_flip = true;
            }
        }
        else if(get_move_dir_x() > 0){
            // Si la velocidad es positiva, significa que se mueve a la derecha
            // y por tanto el sprite debe estar en su posición original
            // y avanzar hacia la derecha sin contar el primer sprite que representa el idle.
            new_src_x =  src_rect.w + (src_rect.x % (get_surface_width() - src_rect.w));
            // Si el sprite está volteado horizontalmente, es necesario volver a voltear
            if(get_horizontal_flip()){
                need_flip = true;
            }
        }
        // Si está en idle y se flipea horizontalmente, es necesario cambiar el src_rect_x
        else if(get_horizontal_flip()){
            new_src_x = get_surface_width() - src_rect.w;
        }

        //printf("%d-%d\n", get_orientation(), counter);

        // Si requiere voltear, se voltea
        if(need_flip){
            flip_surface();
            set_horizontal_flip(!get_horizontal_flip());
        }

        set_src_rect(new_src_x, src_rect.y, src_rect.w, src_rect.h);
        spriteFrameCounter = 0;
    }

    //printf("%d-%d-%d\n", src_rect->x, (src_rect->x + src_rect->w), (src_rect->x + src_rect->w) % get_surface_width());

    spriteFrameCounter++;

}

void Character::jump(){
    
    //get_acceleration().set_y(-20000); Salto varía según valor de dt
    get_velocity().set_y(-700); // Salto no varía según valor de dt

    set_on_air(true);

    printf("SALTO: vel.y = %f\n", get_velocity().get_y());
}

void Character::attack(){

}

void Character::talk(){

}