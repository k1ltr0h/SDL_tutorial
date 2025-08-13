#include "Stage.h"

Stage::Stage(SDL_Window* window_, Player* prota_, Background* background_=NULL){
    window = window_;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    camera = {0, 0, window_w, window_h, {0, 0}}; // Inicializa la cámara con un tamaño por defecto

    prota = prota_;
    objects.push_back(prota);
    background = background_;

    SDL_Surface* bg_surface = background->get_surface();
    clean_surface = SDL_ConvertSurface(bg_surface, bg_surface->format, SDL_SWSURFACE);
}

Stage::~Stage(){
    for(iter = objects.begin(); iter != objects.end(); iter++){
        delete *iter;
    }
}

void Stage::render(){
    background->render(renderer);

    for(iter = objects.begin(); iter != objects.end(); iter++){
        (*iter)->render(renderer);
    }
}

void Stage::update(float dt){
    background->update(dt, camera.offset, objects, false);

    for(iter = objects.begin(); iter != objects.end(); iter++){
        (*iter)->update(dt, camera.offset, objects); // cambiar list por vector
    }

    boundary();

    camera.follow(prota, background->get_surface_width(), background->get_surface_height());
}

void Stage::boundary(){
    Vector2D& prota_pos = prota->get_position();

    int left_boundary = 0;
    int right_boundary = background->get_surface_width();

    if(prota_pos.get_x() < left_boundary){
        prota_pos.set_x(left_boundary);
        prota->stop(GameObject::axis::ABSCISSA);
        prota->update_dst_rect_from_pos(camera.offset); 
    }
    else if(prota_pos.get_x() > right_boundary){
        prota_pos.set_x(right_boundary);
        prota->stop(GameObject::axis::ABSCISSA);
        prota->update_dst_rect_from_pos(camera.offset);
    }
}

void Stage::objects_push_back(GameObject* obj){
    objects.push_back(obj);
}

SDL_Renderer* Stage::get_renderer(){
    return renderer;
}

void Stage::set_background(Background* background_){
    background = background_;
}
