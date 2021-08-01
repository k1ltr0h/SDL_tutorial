#include "GameObject.h"

class Player: public GameObject{
public:
    Player(SDL_Renderer* ren, SDL_Surface* skin_);
    ~Player();

    void render();
    // Getters
    SDL_Surface* get_skin();
private:
    SDL_Surface* skin = NULL;
};