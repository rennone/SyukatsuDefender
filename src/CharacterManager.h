#ifndef __CHARACTER_MANAGER_H__
#define __CHARACTER_MANAGER_H__

#include "Character.h"
#include "Field.h"
#include <syukatsu/syukatsu.h>
#include <math/Vector3.h>

class CharacterManager : public Actor
{
 public:
  CharacterManager(string _name, SyukatsuGame *_game, Field *_field);

  ~CharacterManager() {}

  void update(float deltaTime);
};

#endif
