#include "Player.h"

Player::Player(SDL_Renderer* ren, SDL_Surface* skin_){
    GameObject::init(ren, skin_, 0, 380, 100, 100);
    left = false;
    right = false;
    up = false;
    down = false;
    SDL_SetColorKey(get_skin(), SDL_TRUE, SDL_MapRGB((get_skin())->format, 255, 0, 255));//color de transparencia
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

