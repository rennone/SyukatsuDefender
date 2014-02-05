#include "StrongHold.h"
#include "Field.h"
#include <cmath>
#include <syukatsu/GL/glut.h>

StrongHold::StrongHold(string name, SyukatsuGame *game, Field *_field)
  :Actor(name, game)
  ,time(0)
  ,field(_field)
  ,health(3)
{
}

  StrongHold::~StrongHold()
  {
  }

//包囲攻撃を受ける
void StrongHold::siege()
{
  if( time <= 0 )
  {
    health--;
    time = invisibleTime;
  }
}

bool StrongHold::destroyed() const
{
  return ( health <= 0 && time <= 0);
}

void StrongHold::update(float deltaTime)
{
  if ( time > 0 )
    time = max(time-deltaTime, 0.0f );    
}

void StrongHold::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_MATERIAL | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  glPushMatrix();

  if( time > 0 )
  {
    int n = 10;
    const float alpha = sin(n*2*M_PI*time/invisibleTime);
    float ambient[] = {1.0f, 1.0f, 0.0f, alpha};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ambient);
  }

  float range = Field::cellSize;
  float r0 = range/3;

  //アニメーションが終わってから消えるように, アニメーション中は1増やす
  int hp = health + (time>0);
  for(int i=0; i < hp; i++)
  {
    int num = 2*i+1;
    float r = 1.0*range/hp*i;
    float height = r0*(hp-i+0.5);
    int dDeg = 360/num;
    int deg = 0;
    for(int j=0; j<num; j++)
    {
      glPushMatrix();        
      glTranslatef(r*cos(deg*Vector2::TO_RADIANS),
                   height,
                   r*sin(deg*Vector2::TO_RADIANS));
      glutSolidSphere(r0, 5, 5);
      glPopMatrix();
      deg += dDeg;
    }
  }    
  glPopMatrix();
  glPopAttrib();    
}
