#include EFFECT_H
#define EFFECT_H

#include "Actor.h"
#include <math/Vector3.h>

class Effect:public Actor
{
protected:
  Vector3 position;  
public:
  Effect(string name, SyukatsuGame *game)
    :Actor(name, game)
  {
  }  
  
  virtual ~Effect()
  {
    
  }  
};



#endif
