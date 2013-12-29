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
  static constexpr int fieldSize = 20; //セルサイズ  
  float heightMap[fieldSize+1][fieldSize+1];
  void makeHeightMap();
  void merge(const int &x1, const int &z1, const int &x2, const int &z2);
  void split(const int &x1, const int &z1, const int &x2, const int &z2, const int &n);
  void interpolate(const int &x1, const int &z1, const int &x2, const int &z2);
  float getHeight(const float &x, const float &z) const;
  void cellToVertices(const int &i, const int &j, Vector3 vertices[4]);
  
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

  bool collision(const Vector3 &position, Vector3 &after, const float &radius);
  
};

#endif
