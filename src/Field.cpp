#include "Field.h"
#include "Assets.h"
#include "SimpleObjectFactory.h"
#include <string.h>
#include <iostream>
#include "Debugger.h"
using namespace std;
#include <syukatsu/GL/glut.h>

static vector<Vector2> debugCube;

static Vector3 getTriangleNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
  Vector3 A = v2-v1;
  Vector3 B = v3-v2;
  Vector3 norm = A.cross(B);
//  if(norm.y < 0)
    norm *= -1;
  
  norm.normalize();
  return norm;  
}

//pos1, dir1 の直線と, pos2, dir2の直線との交差判定, cPosは衝突位置, tには dir2の方の衝突時の時間が入る
static bool crossLines2D(const Vector2 &pos1, const Vector2 &dir1, const Vector2 &pos2, const Vector2 &dir2, float &t, Vector2 &cPos)
{
  if( dir1.x == dir2.x && dir1.y == dir2.y)
    return false;

  t = (dir1.y*pos1.x - dir1.x*pos1.y + dir1.x*pos2.y - dir1.y*pos2.x) / (dir1.y*dir2.x - dir1.x*dir2.y);
  cPos = pos2 + dir2*t;
  return true;  
}








bool Field::crossLineTriangle(const Vector3 &tr1, const Vector3 &tr2, const Vector3 &tr3, const Vector3 nor,
                              const Vector3 &pos, const Vector3 &dir, Vector3 &cPos)
{  
  float t = (tr1 - pos).dot(nor)/dir.dot(nor);
  if(t<0)
    return false;
  
  cPos = pos + dir*t;

  //正確に外積使うとずれるから, 暫定的な処理
  return cPos.distanceTo(tr1) < fieldSize || cPos.distanceTo(tr2) < fieldSize || cPos.distanceTo(tr3) < fieldSize;
  
/*
  Vector3 A = tr2 - tr1;
  Vector3 B = tr3 - tr2;
  Vector3 C = tr1 - tr3;

  Vector3 P = (cPos-tr3).cross(C);
  Vector3 Q = (cPos-tr1).cross(A);
  Vector3 R = (cPos-tr2).cross(B);

  return  P.dot(Q) > 0 && P.dot(R) > 0 && Q.dot(R);
*/
}

//--------------------------------------------------------------------------------//
//                                 public                                         //
//--------------------------------------------------------------------------------//

//------------------------------ コンストラクタ ------------------------------//
Field::Field(string name, SyukatsuGame *game, Actor *pmanager, Actor *emanager)
  :Actor(name, game)
  ,position(Vector3(0,0,0))
  ,size(Vector3(1000, 0, 1000))
  ,playerManager(pmanager)
  ,enemyManager(emanager)
{
  cellSize = size.x / (float)fieldSize;  
  makeHeightMap(); //高さマップの自動生成
  bindVBO(); //フィールドの頂点情報をVBO化  
}

///------------------------------ デストラクタ ------------------------------//
Field::~Field()
{  
}

void Field::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

  Assets::textureAtlas->bind();
//  glDisable(GL_DEPTH_TEST);
//  glDisable(GL_ALPHA_TEST);
//  glDisable(GL_BLEND);
  
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[1]);
  glNormalPointer(GL_FLOAT,0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, Vbold[0]);
  glVertexPointer(3, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, Vbold[2]);
  glTexCoordPointer(2,GL_FLOAT, 0, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY); 
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);  

  glDrawArrays(GL_TRIANGLES, 0, 6*fieldSize*fieldSize);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  
  glBindTexture(GL_TEXTURE_2D, 0);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);  
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  Debugger::drawDebugInfo("Field.cpp", "cubeNum", debugCube.size());

  /*
  int i=0;
  float s = debugCube.size();  
  for(auto c : debugCube)
  {
    glPushMatrix();
    glTranslatef(c.x*(cellSize+0.5), 0, c.y*(cellSize+0.5));
    glutSolidCube(cellSize*(s-i++)/s);    
    glPopMatrix();
  }
  */
  
  drawAxis();
  
  glPopAttrib();
}

bool Field::getMouseCollisionPoint(Vector3 &point) const
{
  if(!mouseInRegion)
    return false;

  point = mousePos;
  
  return true;  
}

void Field::updateMousePosition(const Vector3 &position, const Vector3 &direction)
{  
  mouseInRegion = getCollisionPoint(position, direction, mousePos);
}

bool Field::getCollisionPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point)
{  
  //真上からは考えない->動けないようにしてるから
  
  float t1, t2;  
  if( !lineCollision(position, direction, t1, t2) )    
    return false;

  float x1 = (position.x + t1*direction.x)/cellSize;
  float z1 = (position.z + t1*direction.z)/cellSize;
  float x2 = (position.x + t2*direction.x)/cellSize;
  float z2 = (position.z + t2*direction.z)/cellSize;

  float dx = x1 > x2 ? -1 : 1;
  float dz = z1 > z2 ? -1 : 1;
  
  float a = (z2-z1)/(x2-x1);
  float b = z1 - a*x1;
  
  vector<Vector2> cells;  
  const float epsilon = 0.001;
  
  float x = x1;
  float xx1 = dx>0 ? floor(x)+epsilon :  ceil(x)-epsilon;
  float zz1 = min(fieldSize-1.0f, max(0.0f, a*xx1 + b)); 
  while(dx*x <= dx*x2)
  {
    //floor, ceilで値を変えるため, 外に出てしまう可能性があるので, minmaxやってる  
    float xx2 = dx>0 ?  ceil(x)-epsilon : floor(x)+epsilon;
    float zz2 = min(fieldSize-1.0f, max(0.0f, a*xx2 + b));
    float z = dz>0 ? floor(zz1) : ceil(zz1);
    
    while( dz*z < dz*zz2)
    {
      Vector2 cell = Vector2(floor(x), floor(z));
      //cout << cell << endl;
      cells.push_back(cell);
      z += dz;
    }
    
    zz1 = zz2;    
    x += dx;
  } 

  debugCube = cells;   //debug用, 探索するセルを描画する
  
  for(auto cell : cells)    
  {
    int index = (cell.x*fieldSize + cell.y)*3*6;
    Vector3 tr1(vertexBuffer[index], vertexBuffer[index+1], vertexBuffer[index+2]);
    Vector3 tr2(vertexBuffer[index+3], vertexBuffer[index+4], vertexBuffer[index+5]);
    Vector3 tr3(vertexBuffer[index+6], vertexBuffer[index+7], vertexBuffer[index+8]);
    Vector3 tr4(vertexBuffer[index+9], vertexBuffer[index+10], vertexBuffer[index+11]);
    Vector3 nor(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);
    Vector3 cPos;
    
    if(crossLineTriangle(tr1, tr2, tr3, nor, position, direction, point))     
      return true;
    if(crossLineTriangle(tr1, tr4, tr3, nor, position, direction, point))     
      return true;   
  }  

  
  return false;  
}


bool Field::lineCollision(const Vector3 &position, const Vector3 &direction, float &t1, float &t2) const
{  
  const Vector2 nearRight(0, 0);
  const Vector2 nearLeft(size.x, 0);
  const Vector2 farLeft(size.x, size.z);
  const Vector2 farRight(0, size.z);

  Vector2 edge[4];
  edge[0] = nearRight;  edge[1] = nearLeft;
  edge[2] = farLeft;    edge[3] = farRight;

  Vector2 pos(position.x, position.z);
  Vector2 dir(direction.x, direction.z);

  vector<float> colTime;  
  for(int i=0; i<4; i++)
  {
    float t;
    Vector2 cPos;    
    if ( !crossLines2D(edge[i], edge[(i+1)%4]-edge[i], pos, dir, t, cPos) )
      continue;

    const float epsilon = 5; //計算誤差用の余白部分
    
    if(t>0 && cPos.x > -epsilon && cPos.x < size.x+epsilon && cPos.y > -epsilon && cPos.y < size.z+epsilon)
      colTime.push_back(t);      
  }

  if(colTime.size() == 0 || colTime.size() > 2)  
    return false;  

  const float epsilonTime = 5;
  
  if(colTime.size() == 1)
  {
    t1 = 0;
    t2 = colTime[0];    
  }
  else
  {  
    t1 = colTime[0];
    t2 = colTime[1];
    if( t1 > t2)
      swap(t1, t2);
  } 
  
  t1 += epsilonTime;
  t2 -= epsilonTime; 

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
      /*
//      norm[1] = norm[0];
      normalBuffer[normalBufferIndex++] = norm[0].x;
      normalBuffer[normalBufferIndex++] = norm[0].y;
      normalBuffer[normalBufferIndex++] = norm[0].z;
      normalBuffer[normalBufferIndex++] = norm[1].x;
      normalBuffer[normalBufferIndex++] = norm[1].y;
      normalBuffer[normalBufferIndex++] = norm[1].z;
      */
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

  srand(glfwGetTime());  
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


