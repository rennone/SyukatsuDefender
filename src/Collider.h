#ifndef COLLIDER_H_2014_01_01
#define COLLIDER_H_2014_01_01

#include <math/Vector2.h>
class Collider;
class NoCollider;
class CircleCollider;
class RectangleCollider;
class Collider
{
public:
  Vector2 position;
  Collider()
  {
  }
  
  ~Collider() { }  
  virtual bool collisionCheck(const CircleCollider *collider,
                              const Vector2 &before, const Vector2 &after,
                              Vector2 &collisionPos, Vector2 &normal ) const = 0;
  
  virtual bool collisionCheck(const RectangleCollider *collider,
                              const Vector2 &before, const Vector2 &after,
                              Vector2 &collisionPos, Vector2 &normal ) const = 0;  
};

class NoCollider :public Collider
{
public:
  static NoCollider* getNoCollider()
  {
    static NoCollider instance;
    return &instance;    
  }
    
  bool collisionCheck(const CircleCollider *collider,
                              const Vector2 &before, const Vector2 &after,
                              Vector2 &collisionPos, Vector2 &normal ) const;
  
  bool collisionCheck(const RectangleCollider *collider,
                              const Vector2 &before, const Vector2 &after,
                              Vector2 &collisionPos, Vector2 &normal ) const;  
private:
  NoCollider();  
  ~NoCollider();
  NoCollider& operator=(const NoCollider &obj);  
};

class CircleCollider:public Collider
{
public:
  float radius;
  CircleCollider(float _radius);
  ~CircleCollider();  
  
  bool collisionCheck(const CircleCollider *collider,
                      const Vector2 &before, const Vector2 &after,
                      Vector2 &collisionPos, Vector2 &normal ) const;
  
  bool collisionCheck(const RectangleCollider *collider,
                      const Vector2 &before, const Vector2 &after,
                      Vector2 &collisionPos, Vector2 &normal ) const;  
};

#endif

