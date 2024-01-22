#include "Stage.h"

Stage::Stage(SDL_Window* window_, Player* prota_, Background* background_=NULL){
    window = window_;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
    update();
    background->set_surface(SDL_ConvertSurface(clean_surface, clean_surface->format, SDL_SWSURFACE));

    for(iter = objects.begin(); iter != objects.end(); iter++){
        background->blit_surface(*iter);
    }
    render_background();
}

void Stage::render_background(){
    Vector2D* pos = prota->get_current_pos();
    SDL_Rect* bg_rect = background->get_src_rect();

    int sprite_width = bg_rect->w;
    int sprite_height = bg_rect->h;
    int source_x = pos->get_x() - (sprite_width/2);
    int source_y = 0;

    int left_boundary = 0;
    int right_boundary = background->get_surface_width() - sprite_width;

    if(source_x < left_boundary){
        source_x = left_boundary; 
    }
    else if(source_x > right_boundary){
        source_x = right_boundary;
    }

    background->set_src_rect(source_x, source_y, sprite_width, sprite_height);
    background->render(renderer);
}

void Stage::update(){
    for(iter = objects.begin(); iter != objects.end(); iter++){
        (*iter)->update();
    }
    boundary();
}

void Stage::boundary(){
    Vector2D* pos = prota->get_current_pos();

    int left_boundary = 0;
    int right_boundary = background->get_surface_width();

    if(pos->get_x() < left_boundary){
        pos->set_x(left_boundary);
        prota->stop(GameObject::axis::ABSCISSA);
        prota->center(); 
    }
    else if(pos->get_x() > right_boundary){
        pos->set_x(right_boundary);
        prota->stop(GameObject::axis::ABSCISSA);
        prota->center(); 
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