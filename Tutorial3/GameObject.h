#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Vector2D.h"

class GameObject{
public:
    //GameObject();
    virtual ~GameObject();

    void init(SDL_Renderer* ren, Vector2D pos_, int width_, int height_, bool grav_activated_ = true);
    virtual void render();
    virtual void update();
    virtual void move(int dir);
    void stop(int axis);
    void brake(); // frenar
    void ground();
    Vector2D get_position(int t = 1); // Ecuación // t = 1 --> 1 frame
    Vector2D get_velocity(int t = 1); // Ecuación // t = 1 --> 1 frame
    // Getters
    SDL_Texture* get_texture();
    SDL_Renderer* get_renderer();
    Vector2D* get_current_pos();
    Vector2D* get_current_vel();
    Vector2D* get_current_acc();
    // Setters
    void set_texture(SDL_Texture* tex);
    void set_renderer(SDL_Renderer* ren);

    enum dir{RIGHT, LEFT, UP, DOWN};
    enum axis{VERTICAL, HORIZONTAL};

private:
    Vector2D pos;
    Vector2D vel;
    Vector2D acc;
    Vector2D dir;

    int max_velX, max_velY;
    int width, height;

    bool gravity_activated;
    //bool movingX, movingY;

    SDL_Rect src_rect, dst_rect;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
};

