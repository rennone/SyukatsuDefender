#include "Character.h"
#include "GL/glut.h"

Character::Character(string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0,0,0))
  ,direction(Vector3(0,0,1))
{
  radius = 5;  
}

//デフォルトの描画, とりあえずは球体を表示
void Character::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);

  glColor3d(color.x, color.y, color.z);  

  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glutSolidSphere(radius, 10, 10);
  glPopMatrix();

  glPopAttrib();
  
  //子がいれば子の描画を行う
  Actor::render(deltaTime);  
}

void Character::update(float deltaTime)
{

  if(hp <= 0) {
    setStatus(Actor::Dead);
  }

  Actor::update(deltaTime);
}

bool Character::gotDamage(int value)
{
  hp -= value;

  if(hp <= 0) return true;

  return false;
}

bool Character::isHit(const Character* c) 
{
  Vector3 a = position - c->getPosition();

  return a.length() < radius + c->radius;
}

bool Character::collisionCheck(const Vector3 &before, const Vector3 &after, const Character *chara, Vector3 &collisionPos, Vector3 &normal) const
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

bool Character::collisionCheck(const Vector3 &before, const Vector3 &after, const Building *chara, Vector3 &collisionPos, Vector3 &normal) const
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
