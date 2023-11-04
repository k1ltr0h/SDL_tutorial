#include "GameObject.h"

#define G 10 ///< Aceleración de Gravedad: positivo porque 'y' crece hacia abajo en la pantalla.
#define GROUND 450

GameObject::~GameObject(){
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

GameObject::GameObject(SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_, bool collidable_, bool grav_activated_){
    (sur_ != NULL)? surface_width = sur_->w : surface_width = rect_.w;
    (sur_ != NULL)? surface_height = sur_->h : surface_height = rect_.h;
    surface = sur_;

    depth = 10;
    pos = pos_;
    vel = Vector2D();
    acc = Vector2D(0, G);
    max_vel_x = 10, max_vel_y = 200;

    gravity_activated = grav_activated_;
    collidable = collidable_;

    src_rect = rect_;
    dst_rect = {pos.get_x(), pos.get_y(), rect_.w, rect_.h};
}

void GameObject::render(SDL_Renderer* renderer){
    set_texture(SDL_CreateTextureFromSurface(renderer, surface));
    SDL_RenderCopy(renderer, this->texture, &src_rect, &dst_rect);
}

void GameObject::update(bool centered){
    // if is moving acceleration is updated before
    // Update position
    get_position();
    // Update velocity
    get_velocity();
    //printf("Vel: %d-%d\n", vel.getX(), vel.getY());
    //printf("Acc: %d-%d\n", acc.getX(), acc.getY());
    // Hard-coding as ground
    ground();
    // Player brakes, but only is updated if is not moving because velocity updates after move function is called.
    brake();

    if(centered){
        center();
    }
    else{
        dst_rect = {pos.get_x(), pos.get_y(), surface_width, surface_height};
    }
}

void GameObject::move(int dir){
    if(dir == dir::RIGHT){
        acc.set_x(1);
    }
    else if(dir == dir::LEFT){
        acc.set_x(-1);
    }
    else if(dir == dir::UP){
        acc.set_y(1);
    }
    else if(dir == dir::DOWN){
        acc.set_y(-1);
    }

    orientation = dir;
}

void GameObject::stop(int axis){
    if(axis == axis::ORDINATE){
        vel.set_y(0);
        acc.set_y(0);
    }
    else if(axis == axis::ABSCISSA){
        vel.set_x(0);
        acc.set_x(0);
    }
}

void GameObject::brake(){
    Vector2D::Coord curr_vel = vel.get_coord();

    if(curr_vel.x > 0){
        acc.set_x(-1);
    }
    else if(curr_vel.x < 0){
        acc.set_x(1);
    }
    else if(curr_vel.x == 0){
        acc.set_x(0);
    }
    
    if(curr_vel.y < 0){
        acc.set_y(G);
    }
    else if(curr_vel.y == 0 && pos.get_y() >= GROUND){
        acc.set_y(0);
    }
}

void GameObject::ground(){ // Hard-coding for fake ground
    //vel_y = get_velocity();
    //pos_y = get_position();
    
    if(pos.get_y() >= GROUND){
        //vel_y = 0;
        pos.set_y(GROUND);
        on_air = false;
    }
}

void GameObject::center(){
    int half_width = src_rect.w/2;
    int all_height = src_rect.h;
    
    dst_rect = {pos.get_x() - half_width, pos.get_y() - all_height, surface_width, surface_height};
}

void GameObject::blit_surface(GameObject* obj){
    SDL_Rect* obj_src_rect = obj->get_src_rect();
    SDL_Rect* obj_dst_rect = obj->get_dst_rect();
    // Recorta imagen desde sprite set y lo dibuja en rectangulo definido en Last_Surface 
    SDL_BlitSurface(obj->get_surface(), obj_src_rect, surface, obj_dst_rect);
}

Vector2D GameObject::get_position(int t){
    int x = pos.get_x() + vel.get_x()*t + (acc.get_x()/2)*(t*t);
    int y = pos.get_y() + vel.get_y()*t + (acc.get_y()/2)*(t*t);
    pos.set_coord({x, y});
    return pos;
}

Vector2D GameObject::get_velocity(int t){
    int x = vel.get_x() + acc.get_x()*t;
    int y = 0;
    
    if(pos.get_y() < GROUND){ // Hard-coding for fake gravity
        y = vel.get_y() + acc.get_y()*t;
    }

    if(x > max_vel_x){
        x = max_vel_x;
    }
    else if(x < -max_vel_x){
        x = -max_vel_x;
    }

    if(y > max_vel_y){
        y = max_vel_y;
    }
    else if(y < -max_vel_y){
        y = -max_vel_y;
    }

    vel.set_coord({x, y});
    
    return vel;
}

SDL_Surface* GameObject::get_surface(){
    return surface;
}

SDL_Texture* GameObject::get_texture(){
    return texture;
}

Vector2D* GameObject::get_current_pos(){
    return &pos;
}

Vector2D* GameObject::get_current_vel(){
    return &vel;
}

Vector2D* GameObject::get_current_acc(){
    return &acc;
}

SDL_Rect* GameObject::get_src_rect(){
    return &src_rect;
}

SDL_Rect* GameObject::get_dst_rect(){
    return &dst_rect;
}

int GameObject::get_surface_height(){
    return surface_height;
}

int GameObject::get_surface_width(){
    return surface_width;
}

unsigned int GameObject::get_depth(){
    return depth;
}

char GameObject::get_orientation(){
    return orientation;
}

bool GameObject::get_on_air(){
    return on_air;
}

void GameObject::set_surface(SDL_Surface* sur_){
    SDL_FreeSurface(surface);
    surface = sur_;
}

void GameObject::set_texture(SDL_Texture* tex_){
    SDL_DestroyTexture(texture);
    texture = tex_;
}

void GameObject::set_dst_rect(int x_, int y_, int w_, int h_){
    dst_rect = {x_, y_, w_, h_};
}

void GameObject::set_src_rect(int x_, int y_, int w_, int h_){
    src_rect = {x_, y_, w_, h_};
}

void GameObject::set_dst_rect_dimensions(int width_, int height_){
    dst_rect.w = width_;
    dst_rect.h = height_;
}

void GameObject::set_src_rect_dimensions(int width_, int height_){
    src_rect.w = width_;
    src_rect.h = height_;
}

void GameObject::set_dst_rect_position(int x_, int y_){
    dst_rect.x = x_;
    dst_rect.y = y_;
}

void GameObject::set_src_rect_position(int x_, int y_){
    src_rect.x = x_;
    src_rect.y = y_;
}

void GameObject::set_depth(unsigned int depth_){
    depth = depth_;
}

void GameObject::set_orientation(char orientation_){
    orientation = orientation_;
}

void GameObject::set_on_air(bool on_air_){
    on_air = on_air_;
}