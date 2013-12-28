#include "Field.h"
#include "Assets.h"
#include "SimpleObjectFactory.h"
#include <string.h>
#include <iostream>
using namespace std;

#include <syukatsu/GL/glut.h>
Field::Field(string name, SyukatsuGame *game)
  :Actor(name, game)
  ,position(Vector3(0,0,0))
  ,size(Vector3(1000, 0, 1000))
{
  batcher = new SpriteBatcher3D(fieldSize*fieldSize+10);
  /*
  for(int i=0; i<fieldSize; i++)
    for(int j=0; j<fieldSize; j++)
      heightMap[i][j] = rand()%100;  
  */
  makeHeightMap();  
}

Field::~Field()
{
  
}

void Field::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

  Vector3 vertices[4];
  const float cellW = size.x/fieldSize;
  const float cellL = size.z/fieldSize;
  batcher->beginBatch(Assets::textureAtlas);

  for(int i=0; i<fieldSize-1; i++)
    for(int j=0; j<fieldSize-1;j++)
    {
      
      vertices[0] = Vector3(cellW*(i  -fieldSize/2), heightMap[i  ][j  ], cellL*(j  -fieldSize/2));
      vertices[1] = Vector3(cellW*(i+1-fieldSize/2), heightMap[i+1][j  ], cellL*(j  -fieldSize/2));
      vertices[2] = Vector3(cellW*(i+1-fieldSize/2), heightMap[i+1][j+1], cellL*(j+1-fieldSize/2));
      vertices[3] = Vector3(cellW*(i  -fieldSize/2), heightMap[i  ][j+1], cellL*(j+1-fieldSize/2));
      batcher->drawSprite(vertices, Assets::virus);      
    }  
  
  vertices[0] = Vector3(-size.x/2, 0, -size.z/2);
  vertices[1] = Vector3(+size.x/2, 0, -size.z/2);
  vertices[2] = Vector3(+size.x/2, 0, +size.z/2);
  vertices[3] = Vector3(-size.x/2, 0, +size.z/2);
  //batcher->drawSprite(vertices, Assets::virus);      

  batcher->endBatch();  
  drawAxis();  //SimplePbjectFactory  
  
  glPopAttrib();
}

bool Field::getCollisionPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point)
{
  if(direction.y == 0)
    return false;

  float t = -position.y/direction.y;
  float x = position.x + direction.x*t;
  float z = position.z + direction.z*t;

  cout << Vector2(x,z) << endl;
  
  if(x < this->position.x-this->size.x/2 ||
     this->position.x+this->size.x/2 < x ||
     z < this->position.z-this->size.z/2 ||
     this->position.z+this->size.z/2 < z)
    return false;

  point.x = x;
  point.y = 0;
  point.z = z;
  return true;  
}

//pos: 移動前の位置, move: 移動量, after:衝突判定後の位置
//返り値 : 衝突したかどうか
bool Field::collision(const Vector3 &pos, const Vector3 &move, Vector3 &true_after)
{
  const Vector3 after = pos + move;
  
  float left = -size.x/2;  
  if( left > after.x)
  {
    float t = (left-pos.x)/move.x;
    Vector3 point = pos + t*move;    
    point.x -= t*move.x;  // equal to point = point + normal.dot(-move*t)*normal
    true_after = point;
    return true;    
  }

  float right = +size.x/2;
  if( after.x > right)
  {
    float t = (right-pos.x)/move.x;
    Vector3 point = pos + t*move;    
    point.x += t*move.x;  // equal to point = point + normal.dot(-move*t)*normal
    true_after = point;    
    return true;    
  }

  float near =  - size.z/2;
  if( after.z < near)
  {
    float t = (near-pos.z)/move.z;
    Vector3 point = pos + t*move;    
    point.z -= t*move.z;  // equal to point = point + normal.dot(-move*t)*normal
    true_after = point;    
    return true;    
  }  
  
  float far = + size.z/2;
  if( after.z > far)
  {
    float t = (far-pos.z)/move.z;
    Vector3 point = pos + t*move;    
    point.z += t*move.z;  // equal to point = point + normal.dot(-move*t)*normal
    true_after = point;    
    return true;        
  }

  true_after = after;  
  return false;  
}

#include <iostream>
using namespace std;

void Field::makeHeightMap()
{
  for(int i=0; i<fieldSize; i++)
    for(int j=0; j<fieldSize; j++)
      heightMap[i][j] = -1;

  srand(glfwGetTime());  
  // split(0, 0, fieldSize-1, fieldSize-1, 3);
}

void Field::merge(const int &x1, const int &z1, const int &x2, const int &z2)
{
  if(x1==x2 || z1==z2) return;
  int nx = floor((x1+x2)/2), nz = floor((z1+z2)/2);
  heightMap[nx][z1] =  (heightMap[x1][z1]+heightMap[x2][z1])/2;
  heightMap[nx][z2] =  (heightMap[x1][z2]+heightMap[x2][z2])/2;
  heightMap[x1][nz] =  (heightMap[x1][z1]+heightMap[x1][z2])/2;
  heightMap[x2][nz] =  (heightMap[x2][z1]+heightMap[x2][z2])/2;
  bilinearMerge(x1, z1, nx, nz);
  bilinearMerge(x1, nz, nx, z2);
  bilinearMerge(nx, z1, x2, nz);
  bilinearMerge(nx, nz, x2, z2); 
}

void Field::split(const int &x1, const int &z1, const int &x2, const int &z2, const int &n)
{
  if(x1==x2 || z1==z2) return;
  int sum=0;
  int x[4];
  int z[4];
  int maxHeight = 300;
  int randomness= 5;
  x[0] = x[1] = x1; x[2] = x[3] = x2;
  z[0] = z[2] = z1; z[1] = z[3] = z2;
    
  for(int i=0; i<4; i++){
    if(heightMap[x[i]][z[i]] == -1)
      heightMap[x[i]][z[i]] = rand()%maxHeight; 
    sum += heightMap[x[i]][z[i]];
  }
   
  int nx = floor((x1+x2)/2), nz = floor((z1+z2)/2);
  heightMap[nx][nz]     = sum/4 + rand()%randomness;
  heightMap[nx+1][nz]   = sum/4 + rand()%randomness;
  heightMap[nx][nz+1]   = sum/4 + rand()%randomness;
  heightMap[nx+1][nz+1] = sum/4 + rand()%randomness;
  if(n==0){    
    merge(x1,z1,x2,z2);
  }
  else{
    split(x1 ,z1, nx, nz, n-1);
    split(nx ,z1, x2, nz, n-1);
    split(x1 ,nz, nx, z2, n-1);
    split(nx ,nz, x2, z2, n-1);
  }
}

void Field::bilinearMerge(const int &x1, const int &z1, const int &x2, const int &z2)
{
  float d = (x2-x1)*(z2-z1);
  for(int i=x1; i<=x2;i++)
    for(int j=z1;j<=z2;j++)
      heightMap[i][j] = (heightMap[x1][z1]*(x2-i)*(z2-j) +
                 heightMap[x2][z1]*(i-x1)*(z2-j) +
                 heightMap[x1][z2]*(x2-i)*(j-z1) +
                 heightMap[x2][z2]*(i-x1)*(j-z1) )/d;
}



