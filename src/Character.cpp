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

