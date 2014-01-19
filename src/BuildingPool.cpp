#include "BuildingPool.h"

#include <syukatsu/syukatsu.h>
#include "CharacterManager.h"
#include "Field.h"
#include "LightningTower.h"
#include "FreezingTower.h"
#include "Barrack.h"
using namespace Information;

Building* BuildingPool::getInstance(int type, Vector2 cell, SyukatsuGame* game, Field *field, CharacterManager *cManager)
{
  Building *tower;
  switch(type)
  {
  case LIGHTNING_TOWER :
    tower = new LightningTower("lightningTower", game, field, cManager);
    break;
    
  case FREEZING_TOWER :
    tower = new FreezingTower("freezingTower", game, field, cManager);
    break;
    
  case BARRACK :
    tower = new Barrack("barrack", game, field, cManager);
    break;
    
  default:
    return NULL;
  }
  field->setBuilding(tower, cell.x, cell.y);
  tower->setPosition(field->cellToPoint(cell.x, cell.y));
  tower->setPicked(false);
  return tower;  
}

