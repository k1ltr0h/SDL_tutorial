#include "Background.h"

Background::Background(SDL_Window* window_, SDL_Surface* sur_):
Layer(window_, sur_, {0, 0, sur_->w, sur_->h}, Vector2D(sur_->w/2, sur_->h)){
    // Aquí podrías inicializar más cosas específicas del fondo si es necesario
}

Background::~Background(){
}