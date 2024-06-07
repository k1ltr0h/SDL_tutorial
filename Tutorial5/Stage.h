#pragma once
#include <list>
#include <iterator>
#include "Player.h"
#include "Background.h"
#include "Box.h"

class Stage{
public:

    typedef struct{
        bool right = false;
        bool left = false;
        bool above = false;
        bool below = false;
    }CollisionInfo;

    Stage(SDL_Window* window_, Player* prota_, Background* background_);
    virtual ~Stage();
    void render();
    void render_background();
    void update();
    void boundary();
    void objects_push_back(GameObject* obj);
    void detect_collisions();
    void update_before_collition(GameObject* main_obj, GameObject* tmp_obj, CollisionInfo* main_info, CollisionInfo* prev_main_info);
    CollisionInfo* get_collitions(GameObject::HitBox* obj_main_hitbox, GameObject::HitBox* obj_tmp_hitbox);
    //void blit_surface(GameObject* obj);
    virtual void create_scenario()=0;
    //void list_to_list(std::list<GameObject*> list, std::list<GameObject*> otherlist);
    //void save_Collisions(GameObject* obj);

    SDL_Renderer* get_renderer();

    void set_background(Background* background_);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player* prota;
    Background* background;
    SDL_Surface* clean_surface;
    std::list<GameObject*> objects;
    std::list<GameObject*>::iterator iter;
    std::list<GameObject*>::iterator iter2;
};