#include "Box.h"

Box::Box(SDL_Surface* sprite_, Vector2D pos_):
GameObject::GameObject(sprite_, {0,0,60,60}, pos_, true, false){

}