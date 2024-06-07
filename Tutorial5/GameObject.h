#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Vector2D.h"

class GameObject{
public:

    typedef struct{
        int x0;
        int xf;
        int y0;
        int yf;
    }HitBox;

    GameObject(SDL_Surface* sur_=NULL, SDL_Rect rect_={0,0,100,100}, Vector2D pos_=Vector2D(),
    bool collidable_=true, bool grav_activated_=true);
    virtual ~GameObject();

    virtual void render(SDL_Renderer* renderer);
    virtual void update(bool centered=true);
    virtual void move(int dir);
    //void stop_axis(int axis);
    void stop(int axis);
    void brake(); // frenar
    void ground();
    void center();
    void blit_surface(GameObject* obj);
    void flip_surface(bool axis=axis::ABSCISSA);
    // Predict
    Vector2D get_position(int t=1); ///< Ecuación // t = 1 --> 1 frame
    Vector2D get_velocity(int t=1); ///< Ecuación // t = 1 --> 1 frame
    // Getters
    SDL_Surface* get_surface();
    SDL_Texture* get_texture();
    Vector2D* get_current_pos();
    Vector2D* get_prev_pos();
    Vector2D* get_current_vel();
    Vector2D* get_current_acc();
    SDL_Rect* get_src_rect();
    SDL_Rect* get_dst_rect();
    HitBox* get_hit_box();
    HitBox* get_prev_hit_box();
    int get_surface_height();
    int get_surface_width();
    unsigned int get_depth();
    int get_object_type();
    int get_orientation();
    bool is_collidable();
    bool get_on_air();
    bool get_horizontal_flip();
    bool get_vertical_flip();
    bool get_x_axis_collided_right();
    bool get_x_axis_collided_left();
    bool get_y_axis_collided_above();
    bool get_y_axis_collided_below();
    // Setters
    void set_surface(SDL_Surface* sur_);
    void set_texture(SDL_Texture* tex_);

    void set_current_pos(int x_, int y_);
    void set_current_vel(int x_, int y_);
    void set_current_acc(int x_, int y_);

    void set_dst_rect(int x_, int y_, int w_, int h_);
    void set_src_rect(int x_, int y_, int w_, int h_);
    void set_dst_rect_dimensions(int w_, int h_);
    void set_src_rect_dimensions(int w_, int h_);
    void set_dst_rect_position(int x_, int y_);
    void set_src_rect_position(int x_, int y_);

    void set_depth(unsigned int depth_);
    void set_orientation(char orientation_);
    void set_on_air(bool on_air_);
    void set_horizontal_flip(bool horizontal_flip_);
    void set_vertical_flip(bool vertical_flip_);
    void set_x_axis_collided_right(bool is_collide);
    void set_x_axis_collided_left(bool is_collide);
    void set_y_axis_collided_above(bool is_collide);
    void set_y_axis_collided_below(bool is_collide);
    void set_hit_box(HitBox* hitbox_);
    void set_object_type(int type);

    enum dir{RIGHT=0, UP=90, LEFT=180, DOWN=270}; ///< controller (buttons)
    enum axis{ABSCISSA, ORDINATE, ALL}; ///< eje -> x, y, all
    enum type{PLAYER, PLATFORM, NPC};

private:
    SDL_Rect src_rect, dst_rect;
    SDL_Surface* surface = NULL;
    SDL_Texture* texture = NULL;

    Vector2D pos;
    Vector2D prev_pos;
    Vector2D vel;
    Vector2D acc;
    //Vector2D::Coord src_rect_coord;
    //Vector2D::Coord dst_rect_coord;
    HitBox hit_box;
    HitBox prev_hit_box;


    bool gravity_activated;
    bool collidable;
    bool on_air = false;
    bool horizontal_flip = false;
    bool vertical_flip = false;

    //collisions
    bool x_axis_collided_right = false;
    bool x_axis_collided_left = false;
    bool y_axis_collided_above = false;
    bool y_axis_collided_below = false;

    int object_type;
    int orientation; ///< Orientación 0-360°
    int max_vel_x, max_vel_y;
    int surface_width, surface_height;
    //int src_rect_w, src_rect_h;
    //int dst_rect_w, dst_rect_h;
    unsigned int depth; ///< profundidad -> prioridad de dibujado (eje z): valores del 0 al 9 reservados para capas de background.
};

