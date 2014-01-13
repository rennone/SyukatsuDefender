#include "Character.h"
#include "GL/glut.h"
#include "Assets.h"
#include "PlayScene.h"
Character::Character(string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0,0,0))
  ,direction(Vector3(0,0,1))
  ,radius(10)
  ,collider(new CircleCollider(radius))
  ,froze(false)
  ,duration(0)
{
}

//デフォルトの描画, とりあえずは球体を表示
void Character::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);

  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glutSolidSphere(radius, 5, 5);
//  Assets::simpleModel->render();  
  glPopMatrix();
  glPopAttrib();
  
  //子がいれば子の描画を行う
  Actor::render(deltaTime);  
}

void Character::update(float deltaTime)
{

  Vector2 p(position.x, position.z), d(destination.x, destination.z);

  //目的地に到達
  if( p.distanceTo(d) < speed*deltaTime )
  {
    setStatus(Actor::Dead); //たどり着いたら死ぬ    

    //プレイヤーの本拠地へ攻撃する
    ((PlayScene *)(syukatsuGame->getCurrentScene()))->siege();

    //敵の数を減らす
    ((PlayScene *)(syukatsuGame->getCurrentScene()))->decEnemyNum();

    return;    
  }

  //被撃破処理
  if(hp <= 0) {
    setStatus(Actor::Dead);

    ((PlayScene *)(syukatsuGame->getCurrentScene()))->decEnemyNum();
  }

  auto dir = d - p;  
  dir.normalize();

  Vector2 move = dir*deltaTime*speed;
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
