#include "Character.h"

Character::~Character(){}

Character::Character(SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_,
bool collidable_, bool grav_activated_):
GameObject::GameObject(sur_, rect_, pos_, collidable_, grav_activated_){
    
}

void Character::jump(){
    get_current_acc()->add_vector(Vector2D(0, -60));
    set_on_air(true);
}

void Character::attack(){

}

void Character::talk(){

}