#include "Building.h"
#include "GL/glut.h"
#include "Collider.h"
Building::Building(std::string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0, 0, 0))
  ,radius(40)
  ,collider(new CircleCollider(radius))
{
  picked = false;
  setAttributes(100);
}

void Building::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  
  glPushMatrix();

  if(picked) {
    glColor3d(1.0, 0.0, 0.0);
  }
  else {
    glColor3d(0.0, 1.0, 0.0);
  }

  glTranslatef(position.x, position.y, position.z);
  glutSolidCube(radius);
  glPopMatrix();

  glPopAttrib();

  Actor::render(deltaTime);
}

bool Building::collisionCheck(const Vector3 &before, const Vector3 &after, const Character* chara, Vector3 &collisionPos, Vector3 &normal) const
{
  //最初から内部に居る場合は, スルー
  if(before.distanceTo(position) < radius + chara->getRadius())
    return false;

  const Vector3 dir = after - before;
  const Vector3 dR  = position - before;  
  const float a = dir.length();
  const float b = dir.dot(dR);
  const float c = position.distSquared(before) - chara->getRadius() - radius;
  const float D = b*b - a*c;
  
  if( D < 0 || a == 0)
    return false; 

  float t =  -b/a;  

  if( t<0 || t > 1)
    return false;

  collisionPos = before + t*dir;

  normal = collisionPos - position;
  normal.normalize();

  return true;  
}

void Building::update(float deltaTime)
{
  if(hp <= 0) {
    setStatus(Actor::Dead);
  }

  Actor::update(deltaTime);
}

bool Building::gotDamage(int value)
{
  hp -= value;

  if(hp <= 0) return true;

  return false;
}

