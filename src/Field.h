#ifndef FIELD_H_2013_12_26
#define FIELD_H_2013_12_26

#include <math/Vector3.h>
#include <string>
#include "Actor.h"
using namespace std;

class Field:public Actor
{
  const Vector3 position;
  const Vector3 size;
public:
  Field(string name, SyukatsuGame *game);
  ~Field();
  void render(float deltaTime);
  bool getTouchPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point);
};

#endif