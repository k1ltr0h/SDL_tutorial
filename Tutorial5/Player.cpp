#include "Player.h"

Player::Player(SDL_Surface* skin_, Vector2D pos_):
Character::Character(skin_, {0,0,450,482}, pos_){
    SDL_SetColorKey(this->get_surface(), SDL_TRUE,
    SDL_MapRGB((this->get_surface())->format, 255, 0, 255));//color de transparencia
    life = 100;
    set_object_type(GameObject::type::PLAYER);
}

Player::~Player(){

}