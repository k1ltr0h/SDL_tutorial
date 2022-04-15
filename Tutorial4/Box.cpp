#include "Box.h"

Box::Box(SDL_Renderer* ren, SDL_Surface* skin_){
    GameObject::init(ren, skin_, 200, 100, 60, 60);
}