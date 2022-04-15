#include "GameObject.h"

#define G 10 // positivo porque 'y' crece hacia abajo en la pantalla

GameObject::~GameObject(){
    SDL_DestroyTexture(texture);
    pos.~Vector2D();
    vel.~Vector2D();
    acc.~Vector2D();
}

void GameObject::init(SDL_Renderer* ren, Vector2D pos_, int width_, int height_, bool grav_activated_){
    renderer = ren;
    width = width_;
    height = height_;
    pos = pos_;
    vel = Vector2D();
    acc = Vector2D(0, G);
    max_velX = 10, max_velY = 200;
    //movingX = false, movingY= false;
    gravity_activated = grav_activated_;
    dst_rect = {pos.getX(), pos.getY(), width, height};
}

void GameObject::render(){
    SDL_RenderCopy(renderer, this->texture, NULL, &dst_rect);
}

void GameObject::update(){
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

    dst_rect = {pos.getX(), pos.getY(), width, height};
}


void GameObject::move(int dir){
    if(dir == dir::RIGHT){
        acc.setX(1);
        //movingX = true;
    }
    else if(dir == dir::LEFT){
        acc.setX(-1);
        //movingX = true;
    }
    else if(dir == dir::UP){
        acc.setY(1);
        //movingY = true;
    }
    else if(dir == dir::DOWN){
        acc.setY(-1);
        //movingY = true;
    }
    
}

void GameObject::stop(int axis){
    if(axis == axis::VERTICAL){
        vel.setY(0);
        acc.setY(0);
        //movingY = false;
    }
    else if(axis == axis::HORIZONTAL){
        vel.setX(0);
        acc.setX(0);
        //movingX = false;
    }
}

void GameObject::brake(){
    int* curr_vel = vel.getXY();
    //double brakeX = (double)2/max_velX;

    //printf("brakeX: %f %d\n", brakeX, curr_vel[0]);

    //movingX = false;

    if(curr_vel[0] > 0){
        acc.setX(-1);
    }
    else if(curr_vel[0] < 0){
        acc.setX(1);
    }

    else if(curr_vel[0] == 0){
        acc.setX(0);
    }

    if(curr_vel[1] < 0){
        acc.setY(G);
    }

    else if(curr_vel[1] == 0 && pos.getY() >= 380){
        acc.setY(0);
    }

    delete[] curr_vel;
}

void GameObject::ground(){ // Hard-coding for fake ground
    //vel_y = get_velocity();
    //pos_y = get_position();
    
    if(pos.getY() >= 380){
        //vel_y = 0;
        pos.setY(380);
    }
}

Vector2D GameObject::get_position(int t){
    int x = pos.getX() + vel.getX()*t + (acc.getX()/2)*(t*t);
    int y = pos.getY() + vel.getY()*t + (acc.getY()/2)*(t*t);
    pos.setXY(x, y);
    return pos;
}

Vector2D GameObject::get_velocity(int t){
    int x = vel.getX() + acc.getX()*t;
    int y = 0;
    if(pos.getY() < 380){ // Hard-coding for fake gravity
        y = vel.getY() + acc.getY()*t;
    }

    if(x > max_velX){
        x = max_velX;
    }
    else if(x < -max_velX){
        x = -max_velX;
    }

    if(y > max_velY){
        y = max_velY;
    }
    else if(y < -max_velY){
        y = -max_velY;
    }

    vel.setXY(x, y);
    
    return vel;
}

SDL_Texture* GameObject::get_texture(){
    return texture;
}
SDL_Renderer* GameObject::get_renderer(){
    return renderer;
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

void GameObject::set_texture(SDL_Texture* tex){
    texture = tex;
}