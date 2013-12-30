#ifndef __CHARACTER_MANAGER_H__
#define __CHARACTER_MANAGER_H__

#include "PlayerCharacter.h"
#include "Field.h"
#include <syukatsu/syukatsu.h>

class CharacterManager : public PlayerCharacter
{
 protected:

 public:
  CharacterManager(string _name, SyukatsuGame *_game, Field *_field);

  ~CharacterManager() {}

  void update(float deltaTime);
};

#endif
