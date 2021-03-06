#include "Character.h"

class Player: public Character{
public:
    Player(SDL_Renderer* ren, SDL_Surface* skin_);
    ~Player();

    void render();
    // Getters
    SDL_Surface* get_skin();
    void set_directionValue(int dir, bool action);
    int get_directionValue(int dir);
    enum dir{RIGHT, LEFT, UP, DOWN, ALL_H};
private:
    SDL_Surface* skin = NULL;
    bool right;
    bool left;
    bool up;
    bool down;
};