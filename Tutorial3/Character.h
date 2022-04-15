#include "GameObject.h"

class Character: public GameObject{
public:

    virtual ~Character() = 0; // Al ser el destructor virtual, siempre es llamado al destruir un objeto hijo

    virtual void jump();
    virtual void attack();
    virtual void talk();

private:
    int acc_jump;
};