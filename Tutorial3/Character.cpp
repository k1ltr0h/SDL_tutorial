#include "Character.h"

 Character::~Character(){}

void Character::jump(){
    get_current_acc()->addY(-60);
}

void Character::attack(){

}

void Character::talk(){

}