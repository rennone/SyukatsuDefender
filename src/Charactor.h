#ifndef _CHARACTOR_H_2013_12_25
#define _CHARACTOR_H_2013_12_25

#include "Actor.h"

class Charactor : public Actor
{
  protected:
    double x, y, z;

  public:
    Charactor(std::string _name, SyukatsuGame *_game);
    virtual ~Charactor() {}   

    void render(float deltaTime);

    void setPosition(double _x, double _y, double _z) {
	x = _x; y = _y; z = _z;
    }
};

#endif
