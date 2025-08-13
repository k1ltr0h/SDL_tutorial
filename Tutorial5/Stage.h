#pragma once
#include <vector>
#include <iterator>
#include "Player.h"
#include "Background.h"
#include "Box.h"

struct Camera {
    int x, y, w, h;
    Vector2D offset;

    void follow(Player* prota, int stage_w, int stage_h) {
        Vector2D prota_center = prota->get_hit_box().center;
        int half_w = w / 2;
        int half_h = h / 2;
        // Centra la cámara en el personaje
        x = prota_center.get_x();
        y = prota_center.get_y();
        // Ajusta la cámara para que no se salga de los límites del escenario
        x = std::clamp(x, half_w, stage_w - half_w);
        y = std::clamp(y, half_h, stage_h - h);

        offset = {float(x - half_w), float(y)};
    }
};


class Stage{
public:
    Stage(SDL_Window* window_, Player* prota_, Background* background_);
    virtual ~Stage();
    virtual void create_scenario()=0;
    void render();
    void render_background(); 
    void update_camera(); // reemplaza render_background()
    void update(float dt);
    void boundary();
    void objects_push_back(GameObject* obj);
    
    //void list_to_list(std::list<GameObject*> list, std::list<GameObject*> otherlist);
    //void save_Collisions(GameObject* obj);

    SDL_Renderer* get_renderer();

    void set_background(Background* background_);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Camera camera;
    Player* prota;
    Background* background;
    SDL_Surface* clean_surface;
    std::vector<GameObject*> objects;
    std::vector<GameObject*>::iterator iter;
    std::vector<GameObject*>::iterator iter2;
};