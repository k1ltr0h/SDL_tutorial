#include "Layer.h"
#include <list>
#include <iterator>

class Background: public Layer{
public:

    Background(SDL_Window* window_, SDL_Surface* sur_);
    virtual ~Background();
};