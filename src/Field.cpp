#include "Field.h"
#include "Assets.h"
#include "SimpleObjectFactory.h"
#include <string.h>
#include <iostream>
#include "Debugger.h"
using namespace std;
#include <syukatsu/GL/glut.h>

static Vector3 getTriangleNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
  Vector3 A = v2-v1;
  Vector3 B = v3-v2;
  Vector3 norm = A.cross(B);
  if(norm.y < 0)
    norm *= -1;
  
  norm.normalize();
  return norm;  
}

Field::Field(string name, SyukatsuGame *game)
  :Actor(name, game)
  ,position(Vector3(0,0,0))
  ,size(Vector3(1000, 0, 1000))
{  
  makeHeightMap(); //高さマップの自動生成
  bindVBO(); //フィールドの頂点情報をVBO化  
}

Field::~Field()
{  
}

void Field::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

  Assets::textureAtlas->bind();

  glBindBuffer(GL_ARRAY_BUFFER, Vbold[0]);
  glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, Vbold[1]);
  glNormalPointer(GL_FLOAT,0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, Vbold[2]);
  glTexCoordPointer(2,GL_FLOAT, 0, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY); 
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);  

  glDrawArrays(GL_TRIANGLES, 0, 6*fieldSize*fieldSize);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);  
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  
  drawAxis();
  
  glPopAttrib();
}

bool Field::getCollisionPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point)
{
  //真上からの場合, positionそのまま使う
  if(direction.x == 0 && direction.z == 0)
  {
    if(inField(position.x, position.z))
    {      
      point.set(position.x, getHeight(position.x, position.z), position.z);
      return true;      
    }    
    else
    {
      return false;      
    }      
  }
  
  Vector2 pos2(position.x, position.z);
  Vector2 dir2(direction.x,direction.z);
  
  if(direction.x == 0)
  {
    float t1 =      0-position.z/direction.z;
    float t1 = size.z-position.z/direction.z;
  }
  
  float t = -position.y/direction.y;
  float x = position.x + direction.x*t;
  float z = position.z + direction.z*t;
  
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
  int xi = floor(after.x/size.x*fieldSize);
  int zi = floor(after.z/size.z*fieldSize);
  
  if( xi < 0 || xi>=fieldSize || zi < 0 || zi>=fieldSize)
  {
    after = pos;
    return true;
  }  
  else
  {
    after.y = getHeight(after.x, after.z) + radius;
    return false;
  }
}

float Field::getHeight(const float &x, const float &z) const  
{
  const float xf = x/size.x*fieldSize; // x/(size.x/fieldSize) セルインデックスへ変換(float)
  const float zf = z/size.z*fieldSize;

  Vector3 vertices[4];
  cellToVertices(floor(xf), floor(zf), vertices);
  
  int index = (floor(xf)*fieldSize + floor(zf))*3*6;
  
  if( xf-floor(xf) + zf-floor(zf) >= 1)
    index += 9; //2枚目の三角形

  const Vector3 n(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);
  const Vector3 v(vertexBuffer[index], vertexBuffer[index+1], vertexBuffer[index+2]);
  
  return -(n.x*(x-v.x) + n.z*(z-v.z))/n.y + v.y;  
}

void Field::cellToVertices(const int &i, const int &j, Vector3 vertices[4]) const
{
  const float cellW = size.x/fieldSize;
  const float cellL = size.z/fieldSize;
  vertices[0] = Vector3(cellW*i    , heightMap[i  ][j  ], cellL*j    );  //near left
  vertices[1] = Vector3(cellW*(i+1), heightMap[i+1][j  ], cellL*j    );  //near right
  vertices[2] = Vector3(cellW*(i+1), heightMap[i+1][j+1], cellL*(j+1));  //far right
  vertices[3] = Vector3(cellW*i    , heightMap[i  ][j+1], cellL*(j+1));  //far left  
}


void Field::bindVBO()
{

  int vertexBufferIndex= 0;
  int texcoordBufferIndex= 0;
  int normalBufferIndex= 0;
  float texT[6], texV[6];
  texT[0] = texT[3] = texT[5] = Assets::ground->u1;
  texT[1] = texT[2] = texT[4] = Assets::ground->u2;
  texV[0] = texV[1] = texV[3] = Assets::ground->v1;
  texV[2] = texV[4] = texV[5] = Assets::ground->v2;

  Vector3 vert[6];
  Vector3 norm[2];
  
  for(int i=0; i<fieldSize; i++)
  {    
    for(int j=0; j<fieldSize; j++)
    {
      cellToVertices(i, j, vert);
      norm[0] = 
      vert[5] = vert[3];
      vert[3] = vert[0];
      vert[4] = vert[2];
      norm[0] = getTriangleNormal(vert[0], vert[1], vert[2]);
      norm[1] = getTriangleNormal(vert[3], vert[4], vert[5]);
      
      for(int k=0; k<6; k++)
      {
        vertexBuffer[vertexBufferIndex++] = vert[k].x;
        vertexBuffer[vertexBufferIndex++] = vert[k].y;
        vertexBuffer[vertexBufferIndex++] = vert[k].z;

        normalBuffer[normalBufferIndex++] = norm[k/3].x;
        normalBuffer[normalBufferIndex++] = norm[k/3].y;
        normalBuffer[normalBufferIndex++] = norm[k/3].z;

        texcoordBuffer[texcoordBufferIndex++] = texT[k];
        texcoordBuffer[texcoordBufferIndex++] = texV[k];
      }
    }
  } 

  glGenBuffers(3,&Vbold[0]);
  
  //頂点
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

  //法線
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normalBuffer), normalBuffer, GL_STATIC_DRAW);

  //テクスチャ
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordBuffer), texcoordBuffer, GL_STREAM_DRAW);  
}

//============================================================//
//==================== make Terrain ====================//
//============================================================//

void Field::makeHeightMap()
{
  for(int i=0; i<=fieldSize*1; i++)
    for(int j=0; j<=fieldSize*1; j++)
      heightMap[i][j] = -1;

  for(int i=0; i<=fieldSize; i++)  
    heightMap[0][i] = heightMap[fieldSize][i] = heightMap[i][0] = heightMap[i][fieldSize] = 0;    

  split(0, 0, fieldSize*1, fieldSize*1, 2);
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

void Field::interpolate(const int &x1, const int &z1, const int &x2, const int &z2)
{
  //0~1の範囲の値を受け取って, 0~1の範囲の値を返す関数
//  auto func = [](float p)->float{  return sin(p*0.5*M_PI);  };
    auto func = [](float p)->float{  return p*p;  };
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
