#pragma once
#include "Character.h"

class Player: public Character{
public:
    Player(SDL_Surface* skin_, Vector2D pos_ = Vector2D(0, 0));
    ~Player();

private:
    int life;
};