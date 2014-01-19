#ifndef BUILDING_POOL_H
#define BUILDING_POOL_H

#include "Information.h"
#include <math/Vector2.h>
class Building;
class SyukatsuGame;
class Field;
class CharacterManager;

class BuildingPool
{
public:
  static Building* getInstance(int type, Vector2 cell, SyukatsuGame *game, Field *field, CharacterManager *cManager);
};

#endif
