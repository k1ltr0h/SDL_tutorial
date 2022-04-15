#include "Vector2D.h"

Vector2D::Vector2D(){
    x = 0;
    y = 0;
}

Vector2D::Vector2D(int x_, int y_){
    x = x_;
    y = y_;
}

Vector2D::~Vector2D(){
    //delete[] coord;
    //delete[] coord;
}

void Vector2D::addX(int x_){
    x += x_;
}

void Vector2D::addY(int y_){
    y += y_;
}

void Vector2D::multX(int x_){
    x *= x_;
}

int Vector2D::getX(){
    return x;
}

int Vector2D::getY(){
    return y;
}

int* Vector2D::getXY(){ // Recordar liberar memoria luego de llamar esta función
    //int**
    //delete[] coord;
    //coord = new int*[2];
    //coord[0] = &x;
    //coord[1] = &y;
    //int* coord = new int[2];
    coord = new int[2];
    coord[0] = x;
    coord[1] = y;

    return coord;
}

void Vector2D::setX(int x_){
    x = x_;
}

void Vector2D::setY(int y_){
    y = y_;
}

void Vector2D::setXY(int x_, int y_){
    x = x_;
    y = y_;
}

