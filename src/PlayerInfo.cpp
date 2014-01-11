#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(string _name, SyukatsuGame *_game)
  :gold(100)
{
  characterManager = new Actor("aaa", _game);
  buildingManager = new Actor("bbb", _game);
}
  
