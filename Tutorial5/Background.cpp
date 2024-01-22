#include "Background.h"

Background::Background(SDL_Window* window_, SDL_Surface* sur_):
Layer(window_, sur_){
    int win_w, win_h;
    SDL_GetWindowSize(window_, &win_w, &win_h);

    set_src_rect(0, 0, win_w, win_h);
    set_dst_rect(0, 0, win_w, win_h);
}

Background::~Background(){
}