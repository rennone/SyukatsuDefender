#include "Building.h"
#include "GL/glut.h"
#include "Collider.h"
#include "Assets.h"
#include "Information.h"
#include "SimpleObjectFactory.h"
#include "BaseStatus.h"

Building::Building(std::string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0, 0, 0))
  ,radius(20)
  ,collider(new CircleCollider(radius))
  ,level(1)
  ,maxlevel(5)
  ,picked(false)
  ,model(NULL)
  ,fixed(false)
{

  maxhp = 10;
  hp = 10;
}

void Building::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
  
  glPushMatrix();

  if(picked) {
    drawTowerRange();
  }
  
  glTranslatef(position.x, position.y, position.z);

  model->render();  

  glPopMatrix();

  glPopAttrib();

  Actor::render(deltaTime);
}

void Building::setAttributes(int type) 
{

  BuildingBaseStatus* baseStatus = field->getBaseStatus()->getBuildingBaseStatus(type);
  setBaseValue(baseStatus->getBaseValue());
  setAttackRate(baseStatus->getAttackRate());
  setAttack(baseStatus->getAttack());
  setRangeOfEffect(baseStatus->getRangeOfEffect());
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

int Building::getUpgradeCost()
{
  return baseValue + 200 * level ;
}

int Building::getSellValue() 
{
  if(fixed) { 
    return baseValue / 2;
  }

  return baseValue;
}

void Building::drawTowerRange()
{
      glPushAttrib(GL_COLOR_MATERIAL | GL_CURRENT_BIT | GL_ENABLE_BIT); 
      glPushMatrix();      
//      glEnable(GL_ALPHA_TEST);
      glTranslatef(position.x, position.y, position.z);
      
      float col[] = {0.5, 1.0, 1.0, 0.3 };
      glMaterialfv(GL_FRONT, GL_AMBIENT, col);
      Assets::greenRange->texture->bind();
      drawTexture(Vector3(0,2,0), Vector3(0,1,0), calcRange() * 2, Assets::greenRange);
      glBindTexture(GL_TEXTURE_2D, 0);      
      glPopMatrix();
      glPopAttrib();
}

