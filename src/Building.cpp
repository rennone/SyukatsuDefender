#include "Building.h"
#include "GL/glut.h"
#include "Collider.h"
#include "Assets.h"
#include "Information.h"
#include "SimpleObjectFactory.h"

Building::Building(std::string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0, 0, 0))
  ,radius(20)
  ,collider(new CircleCollider(radius))
  ,level(1)
  ,maxlevel(5)
{
  picked = false;
  setAttributes(100);
}

void Building::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  
  glPushMatrix();

  if(picked) {
    drawTowerRange();
  }


  glTranslatef(position.x, position.y, position.z);
//  glutSolidCube(radius);
  Assets::buildings[Information::LIGHTNING_TOWER]->render();  
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

void Building::upgrade()
{
  if(level < maxlevel) { 
    level++;
  }
}

int Building::calcRange() 
{
  return rangeOfEffect + 20 * level;
}

float Building::calcAttackRate()
{
  return attackRate - 0.1 * level;
} 

int Building::getUpgradeCost()
{
  return baseValue + 200 * level ;
}

void Building::drawTowerRange()
{
      glPushAttrib(GL_COLOR_MATERIAL | GL_CURRENT_BIT | GL_ENABLE_BIT); 
      glPushMatrix();      
//      glEnable(GL_ALPHA_TEST);
      glTranslatef(position.x, position.y, position.z);
      
      float col[] = {0.5, 1.0, 1.0, 0.3 };
      glMaterialfv(GL_FRONT, GL_AMBIENT, col);
      Assets::range->texture->bind();
      drawTexture(Vector3(0,2,0), Vector3(0,1,0), rangeOfEffect * 2, Assets::range);
      glBindTexture(GL_TEXTURE_2D, 0);      
      glPopMatrix();
      glPopAttrib();
}

