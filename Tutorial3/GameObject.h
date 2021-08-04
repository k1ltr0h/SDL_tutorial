#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>

class GameObject{
public:
    //GameObject();
    ~GameObject();

    void init(SDL_Renderer* ren, int x_, int y_, int width_, int height_);
    virtual void render();
    void update();
    void gravity();
    int get_position(); // Ecuación
    int get_velocity(); // Ecuación
    // Getters
    int get_x();
    int get_y();
    int get_vel_y();
    SDL_Texture* get_texture();
    SDL_Renderer* get_renderer();
    // Setters
    void set_x(int x_);
    void set_y(int y_);
    void set_vel_y(int vel_y_);
    void set_texture(SDL_Texture* tex);
    void set_renderer(SDL_Renderer* ren);

private:
    int pos_x, pos_y;
    int vel_x, vel_y;
    int ac_x, ac_y;
    int width, height;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
};

