#include "StrongHold.h"
#include "Field.h"
#include <cmath>
#include <syukatsu/GL/glut.h>

StrongHold::StrongHold(string name, SyukatsuGame *game, Field *_field)
  :Actor(name, game)
  ,time(0)
  ,field(_field)
  ,health(6)
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
    //ここでは, healthを減らさずにアニメーションが終わった後に減らす
    time = invisibleTime;
  }
}

bool StrongHold::destroyed() const
{
  return ( health <= 0 && time <= 0);
}

void StrongHold::update(float deltaTime)
{  
  if ( time <= 0 )
    return;
  
  time -= deltaTime;
  if(time <= 0)
  {
    time = 0;
    health--;
  }
}

void StrongHold::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_MATERIAL | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  glPushMatrix();

  float alpha = 1.0f;
  
  if( time > 0 )
    alpha = sin(10*2*M_PI*time/invisibleTime);
  
  float ambient[] = {1.0f, 1.0f, 0.0f, alpha};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ambient);


  float range = Field::cellSize;
  float r0 = range/3;

  int hp = health;
  auto p = [](float p)->float{return sin(p);};
  for(int i=0; i < hp; i++)
  {
    int num = 4*i+1;
    float r =1.0*range*p(1.0*i/hp);
    float height = r0*(hp-i);
    int dDeg = 360/num;
    int deg = 0;
    for(int j=0; j<num; j++)
    {
      glPushMatrix();        
      glTranslatef(r*cos(deg*Vector2::TO_RADIANS),
                   height,
                   r*sin(deg*Vector2::TO_RADIANS));
      glRotatef((i+j+1)*20, i, j, 1);
      glutSolidCube(r0);
      glPopMatrix();
      deg += dDeg;
    }
  }    
  glPopMatrix();
  glPopAttrib();    
}
