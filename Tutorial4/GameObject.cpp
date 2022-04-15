#include "GameObject.h"

#define G 8

GameObject::~GameObject(){
    SDL_DestroyTexture(texture);
}

void GameObject::init(SDL_Renderer* ren, SDL_Surface* skin_, int x_, int y_, int width_, int height_){
    renderer = ren;
    skin = skin_;
    width = width_;
    height = height_;
    pos_x = x_;
    pos_y = y_;
    bound_x = width/2;
    bound_y = height/2;
    center_x = pos_x - bound_x;
    center_y = pos_y - bound_y;
    vel_x = 8;
    vel_y = G;
    ac_x = 0;
    ac_y = G;
    dst_rect = {center_x, center_y, width, height};
}

void GameObject::checkCollisions(GameObject *otherObject){
	int oX = otherObject->get_pos_x();
	int oY = otherObject->get_pos_y();
	int obX = otherObject->get_bound_x();
	int obY = otherObject->get_bound_y();
    /*printf("%d-%d-%d-%d\n",oX,oY,obX,obY);
    printf("%d\n", this->get_ID());
    printf("%d-%d-%d-%d\n",x,y,boundX,boundY);*/
    (pos_x + bound_x + vel_x > oX - obX && pos_x < oX && pos_y > oY - (obY + bound_y) && pos_y < oY + (obY + bound_y)) ? (mv_right = false) : (mv_right = true);
    (pos_x - bound_x - vel_x < oX + obX && pos_x > oX && pos_y > oY - (obY + bound_y) && pos_y < oY + (obY + bound_y)) ? (mv_left = false) : (mv_left = true);
    (pos_y + bound_y + vel_x > oY - obY && pos_y < oY && pos_x > oX - (obX + bound_x) && pos_x < oX + (obX + bound_x)) ? (mv_down = false) : (mv_down = true);
    (pos_y - bound_y - vel_x < oY + obY && pos_y > oY && pos_x > oX - (obX + bound_x) && pos_x < oX + (obX + bound_x)) ? (mv_up = false) : (mv_up = true);
    //Activate Collided function
	if( pos_x + bound_x + vel_x > oX - obX &&
		pos_x - bound_x - vel_x < oX + obX &&
		pos_y + bound_y + vel_y > oY - obY &&
		pos_y - bound_y - vel_y < oY + obY){
        this->collided(otherObject);
    }

}

void GameObject::move_Up(){
    int y = get_pos_y();

    if(y<0){y = 0;}
    else if(y!=0 && get_mv_U()){set_pos_y(y - get_vel_y());}
}
void GameObject::move_Down(){
    int y = get_pos_y();
    if(get_mv_D()){set_pos_y(y + get_vel_y());}
}
void GameObject::move_Left(){
    int x = get_pos_x();
    if(x<0){x = 0;}
    else if(x!=0 && get_mv_L()){set_pos_x(x - get_vel_x());}
}
void GameObject::move_Right(){
    int x = get_pos_x();
    if(get_mv_R()){set_pos_x(x + get_vel_x());}
}

void GameObject::set_mv_R(bool r){
    mv_right = r;
}
void GameObject::set_mv_L(bool l){
    mv_left = l;
}
void GameObject::set_mv_U(bool u){
    mv_up = u;
}
void GameObject::set_mv_D(bool d){
    mv_down = d;
}

void GameObject::collided(GameObject* obj){

}

void GameObject::render(){
    SDL_DestroyTexture(texture);
    set_texture(SDL_CreateTextureFromSurface(renderer, skin));
    SDL_RenderCopy(renderer, this->texture, NULL, &dst_rect);
}

void GameObject::update(){
    gravity();
    //next_PosX = this->img_PosX + this->width;
    center_x = pos_x - width/2;
    center_y = pos_y - height/2;
    dst_rect = {center_x, center_y, width, height};
}

void GameObject::gravity(){

    vel_y = get_velocity();
    pos_y = get_position();
    
    if(pos_y >= 480 - height/2){
        vel_y = 0;
        pos_y = 480 - height/2;
    }
}

bool GameObject::get_mv_R(){
    return mv_right;
}
bool GameObject::get_mv_L(){
    return mv_left;
}
bool GameObject::get_mv_D(){
    return mv_down;
}
bool GameObject::get_mv_U(){
    return mv_up;
}

int GameObject::get_position(){
    return pos_y + vel_y + (ac_y/2);
}

int GameObject::get_velocity(){
    return vel_y + ac_y;
}

int GameObject::get_pos_x(){
    return pos_x;
}
int GameObject::get_pos_y(){
    return pos_y;
}
int GameObject::get_vel_y(){
    return vel_y;
}
int GameObject::get_vel_x(){
    return vel_x;
}
int GameObject::get_bound_x(){
    return bound_x;
}
int GameObject::get_bound_y(){
    return bound_y;
}
SDL_Surface* GameObject::get_skin(){
    return skin;
}
SDL_Texture* GameObject::get_texture(){
    return texture;
}
SDL_Renderer* GameObject::get_renderer(){
    return renderer;
}

void GameObject::set_dstRect(int x_, int y_, int w_, int h_){
    dst_rect = {x_, y_, w_, h_};
}

void GameObject::set_pos_x(int x_){
    pos_x = x_;
}
void GameObject::set_pos_y(int y_){
    pos_y = y_;
}
void GameObject::set_vel_y(int vel_y_){
    vel_y = vel_y_;
}
void GameObject::set_texture(SDL_Texture* tex){
    texture = tex;
}
