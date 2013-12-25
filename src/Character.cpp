#include "Character.h"
#include "GL/glut.h"

Character::Character(string _name, SyukatsuGame *_game)
  :Actor(_name, _game)  
{
}


//デフォルトの描画, とりあえずは球体を表示
void Character::render(float deltaTime)
{
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glutSolidSphere(5, 50, 50);
  glPopMatrix();
  //子がいれば子の描画を行う
  Actor::render(deltaTime);  
}

