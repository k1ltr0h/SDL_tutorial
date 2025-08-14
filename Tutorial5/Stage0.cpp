#include "Stage0.h"

Stage0::Stage0(SDL_Window* window_, Player* prota_):
Stage(window_, prota_, new Background(window_, SDL_LoadBMP("../imgs/stage0.bmp"))){
    create_scenario();

    prota_->set_current_pos(100, 100);
}

void Stage0::create_scenario(){
    objects_push_back(new Platform(SDL_LoadBMP("../imgs/floor1.bmp"), {0, 0, 1280, 32} , Vector2D(640, 480)));
    objects_push_back(new Box(SDL_LoadBMP("../imgs/box.bmp"), Vector2D(400, 200)));
    objects_push_back(new Box(SDL_LoadBMP("../imgs/box.bmp"), Vector2D(600, 300)));
    objects_push_back(new Box(SDL_LoadBMP("../imgs/box.bmp"), Vector2D(800, 400)));
}