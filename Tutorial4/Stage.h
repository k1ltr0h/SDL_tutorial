#include "Layer.h"
#include "Box.h"
#include "Player.h"

class Stage: public Layer{
public:
    Stage(SDL_Renderer* ren, SDL_Surface* skin_, Player* player_);
    void render();
    void update();
    void create_scenario(int id);
    void list_to_list(std::list<GameObject *> list, std::list<GameObject *> otherlist);
    void save_Collisions(GameObject* obj);
    enum scenario{TUTORIAL};
private:
    bool left, right, up, down;
    Player* player;
    std::list<GameObject *> spawn;
    std::list<GameObject *>::iterator iter, iter2;
    std::list<Layer *> layers;
};