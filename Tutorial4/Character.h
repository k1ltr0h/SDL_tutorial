#include "GameObject.h"

class Character: public GameObject{
public:
    Character(SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_, 
    bool collidable_=true, bool grav_activated_=true);
    virtual ~Character() = 0; ///< Al ser el destructor virtual, siempre es llamado al destruir un objeto hijo
    
    //void render(SDL_Renderer* renderer)=0;
    virtual void jump();
    virtual void attack();
    virtual void talk();

private:
    int acc_jump;
};