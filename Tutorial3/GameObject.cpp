#include "GameObject.h"

#define G 10

GameObject::~GameObject(){
    SDL_DestroyTexture(texture);
}

void GameObject::init(SDL_Renderer* ren, int width_, int height_){
    renderer = ren;
    width = width_;
    height = height_;
    pos_x = 0;
    pos_y = 380;
    vel_x = 5;
    vel_y = G;
    ac_x = 0;
    ac_y = G;
    dst_rect = {0, 0, width, height};
}

void GameObject::render(){
    SDL_RenderCopy(renderer, this->texture, NULL, &dst_rect);
}

void GameObject::update(){
    gravity();
    dst_rect = {pos_x, pos_y, width, height};
}

void GameObject::gravity(){

    vel_y = get_velocity();
    pos_y = get_position();
    
    if(pos_y >= 380){
        //printf("o.o");
        vel_y = 0;
        pos_y = 380;
    }
}

int GameObject::get_position(){
    return pos_y + vel_y + (ac_y/2);
}

int GameObject::get_velocity(){
    return vel_y + ac_y;
}

int GameObject::get_x(){
    return pos_x;
}
int GameObject::get_y(){
    return pos_y;
}
int GameObject::get_vel_y(){
    return vel_y;
}
SDL_Texture* GameObject::get_texture(){
    return texture;
}
SDL_Renderer* GameObject::get_renderer(){
    return renderer;
}

void GameObject::set_x(int x_){
    pos_x = x_;
}
void GameObject::set_y(int y_){
    pos_y = y_;
}
void GameObject::set_vel_y(int vel_y_){
    vel_y = vel_y_;
}
void GameObject::set_texture(SDL_Texture* tex){
    texture = tex;
}