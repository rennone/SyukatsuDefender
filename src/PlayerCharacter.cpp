#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
    ,distination(position)
{    
}
