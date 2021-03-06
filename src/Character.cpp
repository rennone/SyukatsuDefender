#include "Character.h"
#include "GL/glut.h"
#include "Assets.h"
#include "PlayScene.h"

#include <iostream>
#include "Debugger.h"
#include "Information.h"
using namespace std;

Character::Character(string _name, SyukatsuGame *_game, Field *_field, int _level)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0,0,0))
  ,direction(Vector3(0,0,1))
  ,radius(10)
  ,curdst(0)
  ,gold(10)
  ,froze(false)  
  ,duration(0)
  ,level(_level)
{ 
  setLane(-1); 
}

void Character::setAttributes(int type)
{
  CharacterBaseStatus* baseStatus = field->getBaseStatus()->getCharacterBaseStatus(type);
  maxhp = calcMaxhp(baseStatus->getMaxhp());
  hp = maxhp;
  attack = baseStatus->getAttack();
  defense = baseStatus->getDefense();
  speed = baseStatus->getSpeed();
}

void Character::setLane(int lane)
{  
  destinations.clear();
  destinations = field->getLane(lane);
}

//デフォルトの描画, とりあえずは球体を表示
void Character::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);

  //ビルボード用
  PlayScene *scene = (PlayScene*)syukatsuGame->getCurrentScene();
  Vector3 cameraPos = scene->getCameraPosition();
  Vector2 dir(0,1);
  Vector2 pos(cameraPos.x- position.x, cameraPos.z - position.z);
  float angle = -dir.angleTo(pos)*Vector2::TO_DEGREE;

  //方向回転用
  Vector3 dst = field->cellToPoint(destinations[curdst].first, destinations[curdst].second);
  Vector2 direction(dst.x-position.x, dst.z-position.z);
  float rotate = direction.angleTo(Vector2(0,-1))*Vector2::TO_DEGREE;

  Debugger::drawDebugInfo("Character.cpp", "angle", angle);
  //キャラクターの表示
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glRotatef(rotate, 0, 1, 0);
//  model->render();

  //キャラクターレベルによって色を変更する
  if(level == 1) {
    model->renderAddColor(0, 0, 1);
  }
  else if(level == 2) {
    model->renderAddColor(1, 0, 0);
  }
  else if(level == 3) {
    model->renderAddColor(0, 1, 0);
  }

  glPopMatrix();

  glDisable(GL_LIGHTING);
  glColor3d(0,1,0);
  
  //hpバーの表示
  glPushMatrix();
  glTranslatef(position.x, position.y + 50, position.z);
  glRotatef(angle, 0, 1, 0);
  glScalef( max(0,hp)/(float)maxhp*radius*3+0.1, 1, 1);
  glutSolidCube(1);  
  glPopMatrix();
  
  glPopAttrib();
  
  //子がいれば子の描画を行う
  Actor::render(deltaTime);  
}

void Character::update(float deltaTime)
{

  //状態異常処理
  if(duration > 0.00) {
    duration -= deltaTime;
  }

  if(duration <= 0.00) {
    duration = 0.00;
    froze = false;
  }

  Vector3 dst = field->cellToPoint(destinations[curdst].first, destinations[curdst].second);
  Vector2 p(position.x, position.z), d(dst.x, dst.z);

  //目的地に到達
  if( p.distanceTo(d) < speed*deltaTime )
  {    
    if(curdst + 1 == destinations.size())
    {
      setStatus(Actor::Dead); //たどり着いたら死ぬ   
      PlayScene* curScene = (PlayScene *)syukatsuGame->getCurrentScene();

      //プレイヤーの本拠地へ攻撃する
      curScene->siege();

      //敵の数を減らす
      curScene->decEnemyNum();
    }
    else {
      curdst++;
    }

    return;    
  }

  //被撃破処理
  if(hp <= 0) {
    setStatus(Actor::Dead);

    PlayScene* curScene = (PlayScene *)syukatsuGame->getCurrentScene();

    curScene->decEnemyNum();
    curScene->addGold(gold);
    curScene->drawGoldString(position, gold);
  }

  //移動処理
  auto dir = d - p;  
  dir.normalize();

  double slow = 1.0;
  if(froze) slow = 0.2;

  Vector2 move = dir * deltaTime * speed * slow;
  Vector2 after = p + move;
  Vector2 cPos, normal;

  /*
  auto list = field->enemyManager->getChildren();
  auto pList = field->playerManager->getChildren();
  list.insert(list.end(), pList.begin(), pList.end());
  
  for (auto child : list)
  {
    if(child->getStatus() != Actor::Action)
      continue;
    
    auto enemy_collider = child->getCollider();
    
    if( enemy_collider->collisionCheck(collider , p, after, cPos, normal) )
    {
      after = cPos + normal*normal.dot(p-after)*1.1;
    }    
  }

  */
  Vector3 after3 = Vector3(after.x, 0, after.y);  
  field->collision(position, after3, radius); 
  position = after3;  

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

bool Character::collisionCheck(const Vector2 &before, const Vector2 &after, const Character *chara, Vector2 &collisionPos, Vector2 &normal) const
{   
  Vector2 pos(position.x, position.z);
  
  //最初から内部に居る場合は, スルー
  if(before.distanceTo(pos) < radius + chara->getRadius())
    return false;
  
  const Vector2 dir = after - before;
  const Vector2 dR  = pos - before;  
  const float a = dir.x*dir.x + dir.y*dir.y;
  const float b = dir.dot(dR);
  const float c = dR.x*dR.x + dR.y*dR.y - pow((radius + chara->getRadius()),2);
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


void Character::gotFrozen()
{
  froze = true;
  duration = 5.00;
}
