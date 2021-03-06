#include "Player.h"

Player::Player(SDL_Renderer* ren, SDL_Surface* skin_){
    GameObject::init(ren, Vector2D(0, 380), 100, 100);
    skin = skin_;
    left = false;
    right = false;
    up = false;
    down = false;
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

void Player::set_directionValue(int dir, bool action){
    if(dir == RIGHT){
        right = action;
    }
    else if(dir == LEFT){
        left = action;
    }
    else if(dir == UP){
        up = action;
    }
    else if(dir == DOWN){
        down = action;
    }
    else if(dir == ALL_H){
        right = action;
        left = action;
    }
}

int Player::get_directionValue(int dir){
    if(dir == RIGHT){
        return right;
    }
    else if(dir == LEFT){
        return left;
    }
    else if(dir == UP){
        return up;
    }
    else if(dir == DOWN){
        return down;
    }

    return 0;
}

