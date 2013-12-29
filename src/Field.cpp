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

  for(int i=0; i<fieldSize; i++)
    for(int j=0; j<fieldSize;j++)
    {
      cellToVertices(i, j, vertices);      
      batcher->drawSprite(vertices, Assets::ground);      
    }
  
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
  
  if(x < this->position.x ||
     this->position.x+this->size.x < x ||
     z < this->position.z ||
     this->position.z+this->size.z < z)
    return false;

  point.x = x;
  point.y = 0;
  point.z = z;
  return true;  
}

//pos: 移動前の位置, move: 移動量, radius: キャラクターの半径, collisionAfter:衝突判定後の位置
//返り値 : 衝突したかどうか
bool Field::collision(const Vector3 &pos, Vector3 &after, const float &radius)
{
  return false;
  
  bool res = false;
  float padding = 0; //余白
  
  float left  = -size.x/2 + padding + radius;
  float right = +size.x/2 - padding - radius;
  if( after.x <= left)
  {
    res = true;
    Vector3 move = after-pos;    
    if(move.x==0)
    {
      after.x = left + 10;      
    }
    else
    {      
      float t = (left-pos.x)/move.x;
      after = pos + t*move;
      after.x -= 1.1*t*move.x;    
    }    
  }
  else if( after.x >= right)
  {
    res = true;
    Vector3 move = after-pos;
    if(move.x==0)
    {
      after.x = right-10;    
    }
    else
    {
      float t = (right-pos.x)/move.x;
      after = pos + t*move;
      after.x += 1.1*t*move.x;  // equal to point = point + normal.dot(-move*t)*normal
    }
  }

  float near = -size.z/2 +padding+radius;
  float far  = +size.z/2 -padding-radius;
  if( after.z <= near)
  {
    res = true;
    Vector3 move = after-pos;
    if(move.z==0)
    {
      after.z = near+10;   
    }
    else
    {
      
      float t = (near-pos.z)/move.z;
      after = pos + t*move;    
      after.z -= 1.1*t*move.z;  // equal to point = point + normal.dot(-move*t)*normal
    }
  }
  else if( after.z >= far)
  {
    res = true;    
    Vector3 move = after-pos;
    if(move.z==0)
    {
      after.z = far-10;    
    }
    else
    {
      float t = (far-pos.z)/move.z;
      after = pos + t*move;    
      after.z += 1.1*t*move.z;  // equal to point = point + normal.dot(-move*t)*normal
    }    
  }
  after.y = getHeight(after.x, after.z);    
  return res;  
}

#include <iostream>
using namespace std;

void Field::makeHeightMap()
{
  for(int i=0; i<fieldSize; i++)
    for(int j=0; j<fieldSize; j++)
      heightMap[i][j] = -1;

  srand(glfwGetTime());
  
//  split(fieldSize*0.1, fieldSize*0.1, fieldSize*0.9, fieldSize*0.9, 3);
//  split(0, 0, fieldSize-1, fieldSize-1, 2);
}

void Field::merge(const int &x1, const int &z1, const int &x2, const int &z2)
{
  if(x1==x2 || z1==z2) return;
  int nx = floor((x1+x2)/2), nz = floor((z1+z2)/2);
  heightMap[nx][z1] =  (heightMap[x1][z1]+heightMap[x2][z1])/2;
  heightMap[nx][z2] =  (heightMap[x1][z2]+heightMap[x2][z2])/2;
  heightMap[x1][nz] =  (heightMap[x1][z1]+heightMap[x1][z2])/2;
  heightMap[x2][nz] =  (heightMap[x2][z1]+heightMap[x2][z2])/2;
  interpolate(x1, z1, nx, nz);
  interpolate(x1, nz, nx, z2);
  interpolate(nx, z1, x2, nz);
  interpolate(nx, nz, x2, z2); 
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

void Field::interpolate(const int &x1, const int &z1, const int &x2, const int &z2)
{
  //0~1の範囲の値を受け取って, 0~1の範囲の値を返す関数
  auto func = [](float p)->float{  return p;  };
  
  const float dx = x2-x1;
  const float dz = z2-z1;
  
  for(int i=x1; i<=x2;i++)
  {    
    for(int j=z1;j<=z2;j++)
    {
      float fx2 = func((x2-i)/dx);
      float fx1 = func((i-x1)/dx);
      float fz2 = func((z2-j)/dz);
      float fz1 = func((j-z1)/dz);
      heightMap[i][j] =
        heightMap[x1][z1]*fx2*fz2 +
        heightMap[x2][z1]*fx1*fz2 +
        heightMap[x1][z2]*fx2*fz1 +
        heightMap[x2][z2]*fx1*fz1;
    }
  }
  
}

float Field::getHeight(const float &x, const float &z) const  
{
  float xf = (x+size.x/2)/fieldSize; //セル空間に変換
  float zf = (z+size.z/2)/fieldSize;
  int ix1 = min(fieldSize-1,max(0, (int)floor(xf)));
  int iz1 = min(fieldSize-1,max(0, (int)floor(zf)));
  int ix2 = min(fieldSize-1,max(0, (int)floor(xf+1)));
  int iz2 = min(fieldSize-1,max(0, (int)floor(zf+1)));
  
  return
    heightMap[ix1][iz1]*(ix2-xf)*(iz2-zf) +
    heightMap[ix2][iz1]*(xf-ix1)*(iz2-zf) +
    heightMap[ix2][iz2]*(xf-ix1)*(zf-iz1) +
    heightMap[ix1][iz2]*(ix2-xf)*(zf-iz1) ;  
}

void Field::cellToVertices(const int &i, const int &j, Vector3 vertices[4])
{
  const float cellW = size.x/fieldSize;
  const float cellL = size.z/fieldSize;
  vertices[0] = Vector3(cellW*i    , heightMap[i  ][j  ], cellL*j    );  //near left
  vertices[1] = Vector3(cellW*(i+1), heightMap[i+1][j  ], cellL*j    );  //near right
  vertices[2] = Vector3(cellW*(i+1), heightMap[i+1][j+1], cellL*(j+1));  //far right
  vertices[3] = Vector3(cellW*i    , heightMap[i  ][j+1], cellL*(j+1));  //far left
}




