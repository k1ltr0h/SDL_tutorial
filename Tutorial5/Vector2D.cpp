#include "Vector2D.h"

Vector2D::Vector2D(){
    coord.x = 0;
    coord.y = 0;
}

Vector2D::Vector2D(float x_, float y_){
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

float Vector2D::get_x() const{
    return coord.x;
}

float Vector2D::get_y() const{
    return coord.y;
}

Vector2D::Coord Vector2D::get_coord(){
    return coord;
}

void Vector2D::set_x(float x_){
    coord.x = x_;
}

void Vector2D::set_y(float y_){
    coord.y = y_;
}

void Vector2D::set_coord(Coord coord_){
    coord.x = coord_.x;
    coord.y = coord_.y;
}

