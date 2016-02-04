#ifndef FIGHT_H
#define FIGHT_H

#include "player.h"
#include "playerstates.h"

class Fight {
  private:
    Player* p;
    Character* c;
    Player* opponent;
    Sprite* currentSprite = NULL;
    PlayerState state = psMATCHSTARTING;
    unsigned dy_reset = 38; //qualitative
    unsigned dy;
    unsigned dx;
    unsigned distance;

    Button away = bNULL;
    Button towards = bNULL;
    Button dir = bNULL;
    int limitCheck(int);

  public:
    Fight(Player*, Player*);
    ~Fight();

    void run();
    Sprite* getSprite();
};

#endif