#include "Stage.h"

Stage::Stage(SDL_Renderer* ren, SDL_Surface* skin_, Player* player_):Layer(ren, skin_){
    player = player_;
    spawn.push_back(player);
    left = true;
    right = true;
    up = true;
    down = true;
    create_scenario(TUTORIAL);
    set_dstRect(0, 0, 640, 480);
}

void Stage::render(){
    GameObject::render();

    for(iter = spawn.begin(); iter != spawn.end(); iter++){
        (*iter)->render();
    }
}

void Stage::update(){

    list_to_list(spawn, spawn);

    for(iter = spawn.begin(); iter != spawn.end(); iter++){
        (*iter)->update();
    }
}

void Stage::create_scenario(int id){
    switch(id){
        case TUTORIAL:
            spawn.push_back(new Box(get_renderer(), SDL_LoadBMP("../imgs/box.bmp")));
            break;
    };
}

void Stage::list_to_list(std::list<GameObject *> list, std::list<GameObject *> otherlist){
    //printf("%d\n",(*iter)->get_X());
    for(iter = list.begin(); iter != list.end(); iter++){
        //printf("%d\n",(*iter)->get_X());
        left = true, right = true, up = true, down = true;
        for(iter2 = otherlist.begin(); iter2 != otherlist.end(); iter2++){
            if((*iter) != (*iter2)){

                //if((*iter)->get_ID() == PLAYER || (*iter)->get_ID() == ENEMY){(*iter) = (Animal*)(*iter);} No es necesario, virtual ya cumple esa función :D
                (*iter)->checkCollisions(*iter2);
                //Esto se utiliza para tener en cuenta multiples colisiones
                save_Collisions(*iter);
            }
        }
        (*iter)->set_mv_R(right);
        (*iter)->set_mv_L(left);
        (*iter)->set_mv_D(down);
        (*iter)->set_mv_U(up);
    }
}
void Stage::save_Collisions(GameObject* obj){
    if(!obj->get_mv_R()){right = false;}
    if(!obj->get_mv_D()){down = false;}
    if(!obj->get_mv_L()){left = false;}
    if(!obj->get_mv_U()){up = false;}
}