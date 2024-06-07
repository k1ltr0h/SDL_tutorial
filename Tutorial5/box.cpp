#include "Box.h"

Box::Box(SDL_Surface* skin_, Vector2D pos_):
GameObject::GameObject(skin_, {0,0,60,60}, pos_, true, false){

}