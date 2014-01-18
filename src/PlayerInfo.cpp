#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(string _name, SyukatsuGame *_game)
  :gold(100)
{
  buildingManager = new Actor("bbb", _game);
}
  
