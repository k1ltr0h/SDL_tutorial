#include "GameObject.h"

class Player: public GameObject{
public:
    Player(SDL_Renderer* ren, SDL_Surface* skin_);
    // Getters
    void set_directionValue(int dir, bool action);
    int get_directionValue(int dir);
    enum dir{RIGHT, LEFT, UP, DOWN, ALL_H};
private:
    bool right;
    bool left;
    bool up;
    bool down;
};