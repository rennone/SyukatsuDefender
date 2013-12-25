#ifndef _CHARACTOR_H_2013_12_25
#define _CHARACTOR_H_2013_12_25

#include "Actor.h"
#include <math/Vector3.h>

class Character : public Actor
{
protected:
  Vector3 position;  

public:
  Character(std::string _name, SyukatsuGame *_game);
  virtual ~Character() {}
  
  virtual void render(float deltaTime);
    
  void setPosition(const float &x, const float &y, const float &z)
  {
    position.set(x,y,z);    
  }

  void setPosition(const Vector3 &pos)
  {
    position = pos;    
  }

  Vector3 getPosition() const
  {
    return position;    
  } 
  
};

#endif
