#include "Stage0.h"

Stage0::Stage0(SDL_Window* window_, Player* prota_):
Stage(window_, prota_, new Background(window_, SDL_LoadBMP("../imgs/stage0.bmp"))){
    create_scenario();
}

void Stage0::create_scenario(){
    objects_push_back(new Box(SDL_LoadBMP("../imgs/box.bmp"), Vector2D(400, 100)));
}