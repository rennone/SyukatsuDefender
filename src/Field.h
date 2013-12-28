#ifndef FIELD_H_2013_12_26
#define FIELD_H_2013_12_26

#include <math/Vector3.h>
#include <syukatsu/syukatsu.h>
#include <string>
#include "Actor.h"
using namespace std;

class Field:public Actor
{
  SpriteBatcher3D *batcher;  
  const Vector3 position;
  const Vector3 size;
  static constexpr int fieldSize = 100; //セルサイズ  
  float heightMap[fieldSize][fieldSize];
  void makeHeightMap();
  void merge(const int &x1, const int &z1, const int &x2, const int &z2);
  void split(const int &x1, const int &z1, const int &x2, const int &z2, const int &n);
  void bilinearMerge(const int &x1, const int &z1, const int &x2, const int &z2);  
public:
  Field(string name, SyukatsuGame *game);
  ~Field();

  void render(float deltaTime);
  bool getCollisionPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point);
  Vector3 getPosition() const
  {
    return position;    
  }

  Vector3 getSize() const
  {
    return size;    
  }

  bool collision(const Vector3 &position,const Vector3 &movement, Vector3 &after);
  
};

#endif
