#include "Layer.h"

Layer::Layer(SDL_Renderer* ren, SDL_Surface* skin_){
    GameObject::init(ren, skin_, 0, 0, 640, 480);
}