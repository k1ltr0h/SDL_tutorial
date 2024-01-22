#include "Character.h"

Character::~Character(){}

Character::Character(SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_,
bool collidable_, bool grav_activated_):
GameObject::GameObject(sur_, rect_, pos_, collidable_, grav_activated_){
    
}

void Character::update(bool centered){
    GameObject::update(centered);

    SDL_Rect* src_rect = get_src_rect();
    Vector2D* vel =  get_current_vel();
    int new_src_x = 0;
    bool need_flip = false;

    //if(counter % 3 == 0){ // each 3 frames
        if(vel->get_x() < 0){ // make the flip
            new_src_x = (src_rect->x + src_rect->w) % (get_surface_width() - src_rect->w);
            if(!get_horizontal_flip()){
                need_flip = true;
            }
        }
        else if(vel->get_x() > 0){
            new_src_x =  src_rect->w + (src_rect->x % (get_surface_width() - src_rect->w));
            if(get_horizontal_flip()){
                need_flip = true;
            }
        }
        else if(get_horizontal_flip()){
            new_src_x = get_surface_width() - src_rect->w;
        }

        //printf("%d-%d\n", get_orientation(), counter);

        if(need_flip){
            flip_surface();
            set_horizontal_flip(!get_horizontal_flip());
        }

        set_src_rect(new_src_x, src_rect->y, src_rect->w, src_rect->h);
        counter = 0;
    //}

    //printf("%d-%d-%d\n", src_rect->x, (src_rect->x + src_rect->w), (src_rect->x + src_rect->w) % get_surface_width());

    counter++;

}

void Character::jump(){
    get_current_acc()->add_vector(Vector2D(0, -60));
    set_on_air(true);
}

void Character::attack(){

}

void Character::talk(){

}