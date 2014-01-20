#include "Character.h"
#include "GL/glut.h"
#include "Assets.h"
#include "PlayScene.h"

#include <iostream>
#include "Debugger.h"
using namespace std;

Character::Character(string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0,0,0))
  ,direction(Vector3(0,0,1))
  ,radius(10)
  ,collider(new CircleCollider(radius))
  ,froze(false)
  ,duration(0)
  ,curdst(0)
  ,gold(10)
{
  setLane(0);
}

void Character::setLane(int lane)
{
  destinations.clear();
  if(lane == 0) {
    destinations.push_back(make_pair(25, 28));
    destinations.push_back(make_pair(23, 27));
    destinations.push_back(make_pair(20, 26));
    destinations.push_back(make_pair(17, 25));
    destinations.push_back(make_pair(14, 24));
    destinations.push_back(make_pair(12, 24));
    destinations.push_back(make_pair( 9, 21));
    destinations.push_back(make_pair( 6, 19));
    destinations.push_back(make_pair( 4, 17));
    destinations.push_back(make_pair( 2, 16));
    destinations.push_back(make_pair( 1, 13));
    destinations.push_back(make_pair( 1, 11));
    destinations.push_back(make_pair( 0,  8));
    destinations.push_back(make_pair( 0,  4));
    destinations.push_back(make_pair( 0,  0));
  }
  else if(lane == 1) {
    destinations.push_back(make_pair(0, 0));
  }
  else if(lane == 2) {
    destinations.push_back(make_pair(29, 28));
    destinations.push_back(make_pair(28, 24));
    destinations.push_back(make_pair(27, 21));
    destinations.push_back(make_pair(26, 19));
    destinations.push_back(make_pair(24, 15));
    destinations.push_back(make_pair(23, 13));
    destinations.push_back(make_pair(21, 10));
    destinations.push_back(make_pair(18,  6));
    destinations.push_back(make_pair(16,  4));
    destinations.push_back(make_pair(13,  2));
    destinations.push_back(make_pair( 9,  0));
    destinations.push_back(make_pair( 0,  0));
  }
  else {
    cout << "invalid lane No. " << lane << endl;
  }
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

  Debugger::drawDebugInfo("Character.cpp", "angle", angle);
  //キャラクターの表示
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glutSolidSphere(radius, 5, 5);
  glPopMatrix();

  glDisable(GL_LIGHTING);
  glColor3d(0,1,0);
//hpバーの表示
  glPushMatrix();
  glTranslatef(position.x, position.y+radius*1.5, position.z);
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

    if(curdst + 1 == destinations.size()) {
      setStatus(Actor::Dead); //たどり着いたら死ぬ    

      //プレイヤーの本拠地へ攻撃する
      ((PlayScene *)(syukatsuGame->getCurrentScene()))->siege();

      //敵の数を減らす
      ((PlayScene *)(syukatsuGame->getCurrentScene()))->decEnemyNum();
    }
    else {
      curdst++;
    }

    return;    
  }

  //被撃破処理
  if(hp <= 0) {
    setStatus(Actor::Dead);

    ((PlayScene *)(syukatsuGame->getCurrentScene()))->decEnemyNum();
  }

  //移動処理
  auto dir = d - p;  
  dir.normalize();

  double slow = 1.0;
  if(froze) slow = 0.2;

  Vector2 move = dir * deltaTime * speed * slow;
  Vector2 after = p + move;
  Vector2 cPos, normal;
  
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
