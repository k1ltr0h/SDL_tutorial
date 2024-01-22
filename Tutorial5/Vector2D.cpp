#include "Vector2D.h"

Vector2D::Vector2D(){
    coord.x = 0;
    coord.y = 0;
}

Vector2D::Vector2D(int x_, int y_){
    coord.x = x_;
    coord.y = y_;
}

Vector2D::~Vector2D(){

}

void Vector2D::add_vector(Vector2D vector){
    coord.x += vector.coord.x;
    coord.y += vector.coord.y;
}

void Vector2D::mult_vector(Vector2D vector){
    coord.x *= vector.coord.x;
    coord.y *= vector.coord.y;
}

int Vector2D::get_x(){
    return coord.x;
}

int Vector2D::get_y(){
    return coord.y;
}

Vector2D::Coord Vector2D::get_coord(){
    return coord;
}

void Vector2D::set_x(int x_){
    coord.x = x_;
}

void Vector2D::set_y(int y_){
    coord.y = y_;
}

void Vector2D::set_coord(Coord coord_){
    coord.x = coord_.x;
    coord.y = coord_.y;
}

