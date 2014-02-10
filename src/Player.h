#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Actor.h"
#include "Field.h"
#include <math/Vector3.h>
#include <syukatsu/SyukatsuGame.h>
#include "CharacterManager.h"

class Player : public Actor
{
  protected:
    int gold;
    float mana;
    float manaregen;

  public:
    Player(string _name, SyukatsuGame *_game, int initialGold);
    ~Player() {}

    int getGold() { return gold; }
    void addGold(const int value) { gold += value; } 
    void subGold(const int value) { gold -= value; }

    bool canMagicAttack();
    void castFireball();
    
    float getMana() { return mana; }

    void render(float deltaTime);
    void update(float deltaTime);
};

#endif
  
