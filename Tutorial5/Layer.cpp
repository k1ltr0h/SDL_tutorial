#include "Layer.h"

Layer::Layer(SDL_Window* window_, SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_, unsigned int depth_):
GameObject(sur_, rect_, pos_){
    surface = sur_;
    (sur_ != NULL)? width = sur_->w : width = rect_.w;
    (sur_ != NULL)? height = sur_->h : height = rect_.h;
    depth = depth_;
}

Layer::~Layer(){
    
}

void Layer::set_depth(int depth_){
    depth = depth_;
}

int Layer::get_depth(){
    return depth;
}