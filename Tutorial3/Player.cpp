#include "Player.h"

Player::Player(SDL_Renderer* ren, SDL_Surface* skin_){
    GameObject::init(ren, 0, 380, 100, 100);
    skin = skin_;
    SDL_SetColorKey(this->skin, SDL_TRUE, SDL_MapRGB((this->skin)->format, 255, 0, 255));//color de transparencia
}

Player::~Player(){
    SDL_FreeSurface(skin);
}

void Player::render(){
    SDL_DestroyTexture(get_texture());
    set_texture(SDL_CreateTextureFromSurface(get_renderer(), skin));
    GameObject::render();
}

SDL_Surface* Player::get_skin(){
    return skin;
}

