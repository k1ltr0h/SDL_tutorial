#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <list>

class GameObject{
public:
    //GameObject();
    ~GameObject();

    void init(SDL_Renderer* ren, SDL_Surface* skin_, int x_, int y_, int width_, int height_);
    virtual void render();
    virtual void update();
    void gravity();
    int get_position(); // Ecuación
    int get_velocity(); // Ecuación
    // Getters
    int get_pos_x();
    int get_pos_y();
    int get_vel_y();
    int get_vel_x();
    int get_bound_x();
    int get_bound_y();
    bool get_mv_R(); //
    bool get_mv_L(); //
    bool get_mv_D(); //
    bool get_mv_U();
    SDL_Surface* get_skin();
    SDL_Texture* get_texture();
    SDL_Renderer* get_renderer();
    // Setters
    void set_pos_x(int x_);
    void set_pos_y(int y_);
    void set_vel_y(int vel_y_);
    void set_texture(SDL_Texture* tex);
    void set_renderer(SDL_Renderer* ren);
    void set_dstRect(int x_, int y_, int w_, int h_);
    virtual void collided(GameObject* obj);
    void checkCollisions(GameObject *otherObject);

    virtual void move_Up();
    virtual void move_Down();
    virtual void move_Left();
    virtual void move_Right();

    void set_mv_R(bool r);
    void set_mv_L(bool l);
    void set_mv_U(bool u);
    void set_mv_D(bool d);

private:
    bool mv_up, mv_down, mv_left, mv_right;//! Sirve para las colisiones, permite movimiento es esa direccion
    int pos_x, pos_y;
    int center_x, center_y;
    int bound_x, bound_y;
    int vel_x, vel_y;
    int ac_x, ac_y;
    int width, height;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    SDL_Surface* skin = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
};

