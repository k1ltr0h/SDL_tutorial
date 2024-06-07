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
    detect_collisions();
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

void Stage::detect_collisions(){

    for(iter = objects.begin(); iter != objects.end(); iter++){

        if(!(*iter)->is_collidable() || (*iter)->get_object_type() == GameObject::type::PLATFORM){
            continue;
        }

        for(iter2 = objects.begin(); iter2 != objects.end(); iter2++){
            if((*iter) != (*iter2) && ((*iter2)->is_collidable() || (*iter)->get_object_type() == GameObject::type::PLATFORM)){

                //if((*iter)->get_current_pos()->get_y() == 200){
                //    printf("x-axis: %d-%d <-> %d-%d\n", obj_tmp_hitbox_x0, obj_tmp_hitbox_xf, 
                //    obj_main_hitbox_x0, obj_main_hitbox_xf);
                //    printf("y-axis: %d-%d <-> %d-%d\n", obj_tmp_hitbox_y0, obj_tmp_hitbox_yf, 
                //    obj_main_hitbox_y0, obj_main_hitbox_yf);
                //}

                CollisionInfo* main_obj_info = get_collitions((*iter)->get_hit_box(), (*iter2)->get_hit_box());
                CollisionInfo* main_obj_prev_info = get_collitions((*iter)->get_prev_hit_box(), (*iter2)->get_hit_box());

                //printf("x-axis: %d-%d <-> y-axis: %d-%d\n", x_axis_collided_right, x_axis_collided_left, 
                //y_axis_collided_above, y_axis_collided_below);

                //if((*iter)->get_current_pos()->get_y() != 200)
                //    printf("x-axis: %d <-> y-axis: %d\n", 
                //    (*iter)->get_current_pos()->get_x(),
                //    (*iter)->get_current_pos()->get_y());

                update_before_collition((*iter), (*iter2), main_obj_info, main_obj_prev_info);

                delete main_obj_info;
                delete main_obj_prev_info;
            }
        }
    }
}

void Stage::update_before_collition(GameObject* main_obj, GameObject* tmp_obj, 
CollisionInfo* main_info, CollisionInfo* prev_main_info){

    printf("prev: %d-%d-%d-%d\n", prev_main_info->right, prev_main_info->left,
    prev_main_info->above, prev_main_info->below);
    printf("current: %d-%d-%d-%d\n\n", main_info->right, main_info->left,
    main_info->above, main_info->below);

    //if(prev_main_info->right || prev_main_info->left){
        if(main_info->above){
            printf("Above\n");
            main_info->above = false;
            //(*iter)->stop(1);
            main_obj->set_current_vel(main_obj->get_current_vel()->get_x(), 0);
            main_obj->set_current_pos(main_obj->get_current_pos()->get_x(),
            tmp_obj->get_current_pos()->get_y() + main_obj->get_dst_rect()->h + 1);
        }

        if(main_info->below){
            printf("Below\n");
            main_info->below = false;
            //(*iter)->stop(1);
            main_obj->set_current_vel(main_obj->get_current_vel()->get_x(), 0);
            main_obj->set_current_pos(main_obj->get_current_pos()->get_x(),
            tmp_obj->get_current_pos()->get_y() - tmp_obj->get_dst_rect()->h - 1);
            main_obj->set_on_air(false);
        }
    //}
    //if(prev_main_info->above || prev_main_info->below){
        if(main_info->right){
            printf("Right\n");
            main_info->right = false;
            main_obj->set_current_vel(0, main_obj->get_current_vel()->get_y());
            main_obj->set_current_pos(tmp_obj->get_current_pos()->get_x() - tmp_obj->get_dst_rect()->w/2 - main_obj->get_dst_rect()->w/2 - 1,
            main_obj->get_current_pos()->get_y());
        }
        if(main_info->left){
            printf("Left\n");
            main_info->left = false;
            main_obj->set_current_vel(0, main_obj->get_current_vel()->get_y());
            main_obj->set_current_pos(tmp_obj->get_current_pos()->get_x() + tmp_obj->get_dst_rect()->w/2 + main_obj->get_dst_rect()->w/2 + 1,
            main_obj->get_current_pos()->get_y());
        }
    //}
    //if(main_obj->get_current_pos()->get_y() != 200)
    //printf("x-axis: %d <-> y-axis: %d\n\n", 
    //main_obj->get_current_pos()->get_x(), 
    //main_obj->get_current_pos()->get_y());

    //main_obj->set_x_axis_collided_right(x_axis_collided_right);
    //main_obj->set_x_axis_collided_left(x_axis_collided_left);
    //main_obj->set_y_axis_collided_above(y_axis_collided_above);
    //main_obj->set_y_axis_collided_below(y_axis_collided_below);

    main_obj->center();
}

Stage::CollisionInfo* Stage::get_collitions(GameObject::HitBox* obj_main_hitbox, GameObject::HitBox* obj_tmp_hitbox){
    bool x_axis_collided_right, x_axis_collided_left, y_axis_collided_above, y_axis_collided_below;
    int tmp_half_width = (obj_tmp_hitbox->xf - obj_tmp_hitbox->x0);
    int tmp_half_height = (obj_tmp_hitbox->yf - obj_tmp_hitbox->y0)/2;

    x_axis_collided_right = false;
    x_axis_collided_left = false;
    y_axis_collided_above = false;
    y_axis_collided_below = false;

    // maybe prove with SDL_IntersectRect()? nah!
    
    // Is x Object collided with main object?
    if(obj_tmp_hitbox->y0 > obj_main_hitbox->y0 - tmp_half_height &&
       obj_tmp_hitbox->yf < obj_main_hitbox->yf + tmp_half_height){
        if(obj_main_hitbox->x0 >= obj_tmp_hitbox->x0 &&
            obj_main_hitbox->x0 <= obj_tmp_hitbox->xf){
            x_axis_collided_left = true;
        }
        if(obj_main_hitbox->xf >= obj_tmp_hitbox->x0 &&
            obj_main_hitbox->xf <= obj_tmp_hitbox->xf){
            //printf("%s\n", "Wena po Barnie!");
            x_axis_collided_right = true;
        }
    }
    if(obj_tmp_hitbox->x0 >= obj_main_hitbox->x0 - tmp_half_width &&
       obj_tmp_hitbox->xf <= obj_main_hitbox->xf + tmp_half_width){
        if(obj_main_hitbox->y0 >= obj_tmp_hitbox->y0 &&
            obj_main_hitbox->y0 <= obj_tmp_hitbox->yf /*&&
            obj_main_hitbox->y0 - obj_tmp_hitbox->y0 >= tmp_half_width*/){
            y_axis_collided_above = true;
        }
        if(obj_main_hitbox->yf >= obj_tmp_hitbox->y0 &&
            obj_main_hitbox->yf <= obj_tmp_hitbox->yf /*&&
            obj_tmp_hitbox->yf - obj_main_hitbox->xf >= tmp_half_width*/){
            //printf("%s\n", "Wena po Mordelon!");
            y_axis_collided_below = true;
        }
    }

    return new CollisionInfo {x_axis_collided_right, x_axis_collided_left, y_axis_collided_above, y_axis_collided_below};
}

SDL_Renderer* Stage::get_renderer(){
    return renderer;
}

void Stage::set_background(Background* background_){
    background = background_;
}