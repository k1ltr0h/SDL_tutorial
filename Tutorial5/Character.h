#include "GameObject.h"

class Character: public GameObject{
public:
    Character(SDL_Surface* sur_, SDL_Rect rect_, Vector2D pos_ = Vector2D(0, 0),
              bool collidable_=true, bool grav_activated_=true);
    virtual ~Character() = 0; ///< Al ser el destructor virtual, siempre es llamado al destruir un objeto hijo
    
    //void render(SDL_Renderer* renderer)=0;
    virtual void update(float dt, Vector2D& camera_offset, const std::vector<GameObject*>& allObjects) override;
    virtual void animate();
    virtual void jump();
    virtual void attack();
    virtual void talk();


private:
    int spriteFrameCounter;
    int acc_jump;
};