#include "Collider.h"


//------------------------------ NoCollider------------------------------
NoCollider::NoCollider()
  :Collider()
{
}

NoCollider::~NoCollider()
{
}

bool NoCollider::collisionCheck(const CircleCollider *collider,
                              const Vector2 &before, const Vector2 &after,
                              Vector2 &collisionPos, Vector2 &normal ) const
{
  return false;  
}

bool NoCollider::collisionCheck(const RectangleCollider *collider,
                              const Vector2 &before, const Vector2 &after,
                              Vector2 &collisionPos, Vector2 &normal ) const
{
  return false;  
}


//------------------------------CircleCollider------------------------------//
CircleCollider::CircleCollider(float _radius)
  :Collider()
  ,radius(_radius)
{
}

CircleCollider:: ~CircleCollider()
{
}

bool CircleCollider::collisionCheck(const CircleCollider *collider,
                                    const Vector2 &before, const Vector2 &after,
                                    Vector2 &collisionPos, Vector2 &normal ) const
{
  Vector2 pos(position.x, position.y);

  const float R = radius + collider->radius;
  
  //最初から内部に居る場合は, スルー
  if(before.distanceTo(pos) < R)
    return false;
  
  const Vector2 dir = after - before;
  const Vector2 dR  = pos - before;  
  const float a = dir.x*dir.x + dir.y*dir.y;
  const float b = dir.dot(dR);
  const float c = dR.x*dR.x + dR.y*dR.y - pow(R,2);
  const float D = b*b - a*c;

  if( D < 0 || a == 0)
    return false; 
  
  float t1 =  (b+sqrt(D))/a;  
  float t2 =  (b-sqrt(D))/a;
  
  if( t1 < 0 || t2 < 0)
    return false;
  
  float t = min(t1, t2);
//  cout << t << endl;
  
  if( t<0 || t > 1)    return false;

  collisionPos = before + t*dir;

  normal = collisionPos - pos;
  normal.normalize();

  return true;  
}


bool CircleCollider::collisionCheck(const RectangleCollider *collider,
                                    const Vector2 &before, const Vector2 &after,
                                    Vector2 &collisionPos, Vector2 &normal ) const
{
  return false;  
}

