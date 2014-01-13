#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "Character.h"

class Knight : public Character
{
  public:
    Knight(string name, SyukatsuGame *game, Field *field);  
    ~Knight() {  }   

  void update(float deltaTime);
};


#endif
