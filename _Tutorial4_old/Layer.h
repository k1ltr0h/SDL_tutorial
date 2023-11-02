#include "GameObject.h"

class Layer: public GameObject{
public:
    Layer(SDL_Renderer* ren, SDL_Surface* skin);
};