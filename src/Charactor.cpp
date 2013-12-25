#include "Charactor.h"
#include "GL/glut.h"


Charactor::Charactor(string _name, SyukatsuGame *_game)
  :Actor(_name, _game)
{
}

void Charactor::render(float deltaTime)
{
  glPushMatrix();
  glTranslatef(30, 0, 0);
  glutSolidSphere(5, 100, 100);
  glPopMatrix();

  glFlush();
  //this->render(deltaTime);
}

