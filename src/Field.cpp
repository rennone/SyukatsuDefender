#include <GL/glew.h>
#include "Field.h"
#include "Assets.h"
#include "Building.h"

//#include "SimpleObjectFactory.h"
//#include "Debugger.h"
//#include <syukatsu/GL/glut.h>
//#include <syukatsu/syukatsu.h>
#include <string.h>
#include <iostream>
using namespace std;

static vector<Vector2> debugCube;

static Vector3 getTriangleNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
  Vector3 A = v2-v1;
  Vector3 B = v3-v2;
  Vector3 norm = A.cross(B);
  
  if(norm.y < 0)    norm *= -1;
  
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
  return cPos.distanceTo(tr1) < cellSize || cPos.distanceTo(tr2) < cellSize || cPos.distanceTo(tr3) < cellSize;
  
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
  ,size(Vector3(cellNum*cellSize, 0, cellNum*cellSize))
  ,playerManager(pmanager)
  ,enemyManager(emanager)
{
  for(int i=0; i<cellNum; i++)
    for(int j=0; j<cellNum; j++)
      buildingInMap[i][j] = NULL;
  
  memset(buildingInField, -1, sizeof(buildingInField));  
  makeHeightMap(); //高さマップの自動生成
  createMapChip();
  bindVBO(); //フィールドの頂点情報をVBO化  
}

///------------------------------ デストラクタ ------------------------------//
Field::~Field()
{  
}

//--------------------render--------------------//
void Field::render(float deltaTime)
{  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_MATERIAL);

  Assets::textureAtlas->bind();  
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[1]);
  glNormalPointer(GL_FLOAT,0, 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[0]);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[2]);
  glTexCoordPointer(2,GL_FLOAT, 0, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY); 
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);  

  glDrawArrays(GL_TRIANGLES, 0, 6*cellNum*cellNum);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);//必要  
  glBindTexture(GL_TEXTURE_2D, 0); //必要 これがないと, ただの球とか,この下のGL_TRIANGLEが消える(見えなくなってる?)  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);  
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  if(mouseInRegion)
  {
    Vector2 cell;  
    getMouseCollisionCell(cell);

    if( isValidPosition(cell.x, cell.y) )
    {
      float col[] = {0.0, 1.0, 0.0};    
      glMaterialfv(GL_FRONT, GL_AMBIENT, col);
    }
    else
    {
      float col[] = {1.0, 0.0, 0.0};    
      glMaterialfv(GL_FRONT, GL_AMBIENT, col);
    }
    int ind = cellToIndex(cell.x, cell.y);

//todo これでは重い?    
    glBegin(GL_TRIANGLES);
    for(int i=0; i<16; i+=3)
      glVertex3d(vertexBuffer[ind+i+0]+normalBuffer[ind+i+0]*0.1,
                 vertexBuffer[ind+i+1]+normalBuffer[ind+i+1]*0.1,
                 vertexBuffer[ind+i+2]+normalBuffer[ind+i+2]*0.1);
    glEnd();    
  }
  
  glPopAttrib();
    
//  drawAxis();  //軸の描画 todo 最後はいらない
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

}

Vector3 Field::cellToPoint(const int &i, const int &j) const
{
  const float x = (i+0.5)*cellSize;
  const float z = (j+0.5)*cellSize;
  
  const float y = getHeight(x,z);
  return Vector3(x,y,z);
}

pair<float, float> Field::pointToCell(const Vector3& v) 
{
  float x = (v.x - 0.5) / cellSize;
  float y = (v.z - 0.5) / cellSize;

  return make_pair(x, y);
}

//マウスが指しているフィールドの位置を取得
bool Field::getMouseCollisionCell(Vector2 &cell) const
{
  if(!mouseInRegion)
    return false;

  cell.x = floor(mousePos.x/cellSize);
  cell.y = floor(mousePos.z/cellSize);

  Vector3 nor1, nor2;  
  getNormalVectorInCell(cell.x, cell.y, nor1, nor2);
  
  if(nor1.dot(Vector3(0,1,0)) < 0.9 || nor2.dot(Vector3(0,1,0)) < 0.9)
   return false;
  
  return true;  
}

void Field::unPickedBuildingAll()
{
  for(int i = 0; i < cellNum; ++i) {
    for(int j = 0; j < cellNum; ++j) {
      if(buildingInMap[i][j] != NULL) {
	buildingInMap[i][j]->setPicked(false);
      }
    }
  }
}

void Field::pickBuilding(const int &i, const int &j) 
{
  unPickedBuildingAll();
  if(buildingInMap[i][j] == NULL) {
    return;
  }

  buildingInMap[i][j]->setPicked(true);
}

void Field::setBuildingInField(const Vector2 &cell, const int &kind)
{
  int x = (int)(cell.x);
  int y = (int)(cell.y);
  
  buildingInField[x][y] = kind;  
}

bool Field::setBuilding(Building *build, const int &i, const int &j)
{
  if(i < 0 || j < 0 || i >= cellNum || j >= cellNum || build == NULL)
    return false;

  //通路にはとりあえず置けない
  if( mapchip[i][j] != Bush) {
    return false;  
  }

  //すでに,建物があれば置けない
  if(buildingInMap[i][j] != NULL) {
    return false;
  }
  
  buildingInMap[i][j] = build;
  return true;  
}

bool Field::isValidPosition(const int i, const int j) {
  if(i < 0 || j < 0 || i >= cellNum || j >= cellNum)
    return false;

  //通路にはとりあえず置けない
  if( mapchip[i][j] != Bush) {
    return false;  
  }

  //すでに,建物があれば置けない
  if(buildingInMap[i][j] != NULL) {
    return false;
  }

  return true;
}

void Field::deleteBuilding(const int &i, const int &j)
{
  if(i<0 || j<0 || i>=cellNum || j>= cellNum)
    return;

  buildingInMap[i][j]->setStatus(Actor::Dead);
  buildingInMap[i][j] = NULL;  
}

void Field::deleteBuilding()
{
  for(int i = 0; i < cellNum; ++i) {
    for(int j = 0; j < cellNum; ++j) {
      if(buildingInMap[i][j] == NULL) continue;

      if(buildingInMap[i][j]->getPicked()) {
	deleteBuilding(i, j);
      }
    }
  }
}

void Field::upgradeBuilding()
{
}

Building* Field::getBuilding(const int &i, const int &j)
{
  if(i < 0 || j < 0 || i >= cellNum || j >= cellNum) {
    return NULL;
  }

  return buildingInMap[i][j];  
}

//マウスが指しているフィールドの位置を取得
bool Field::getMouseCollisionPoint(Vector3 &point) const
{
  if(!mouseInRegion)
    return false;
  
  point = mousePos;
  return true;  
}

//マウスが指しているフィールドの位置を計算(毎フレームの最初に呼び出す)
void Field::updateMousePosition(const Vector3 &position, const Vector3 &direction)
{  
  mouseInRegion = getCollisionPoint(position, direction, mousePos);
  Vector2 cell;
  
  cell.x = floor(mousePos.x/cellSize);
  cell.y = floor(mousePos.z/cellSize);

//  Debugger::drawDebugInfo("Field.cpp", "mousePos", mouseInRegion);  
}

//
bool Field::getCollisionPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point)
{  
  //四辺との衝突点を求める(真上からは考えない)
  float t1, t2;
  if( !lineCollision(position, direction, t1, t2) )    
    return false;

  //衝突点のセル座標
  float x1 = (position.x + t1*direction.x)/cellSize;
  float z1 = (position.z + t1*direction.z)/cellSize;
  float x2 = (position.x + t2*direction.x)/cellSize;
  float z2 = (position.z + t2*direction.z)/cellSize;

  //手前から調べる必要があるので,方向も考えた直線の式を求める
  float dx = x1 > x2 ? -1 : 1;
  float dz = z1 > z2 ? -1 : 1;  
  float a = (z2-z1)/(x2-x1); //一次関数の係数
  float b = z1 - a*x1;       //
  
  vector<Vector2> cells;  
  const float epsilon = 0.001;
  
  float x = x1;
  float xx1 = dx>0 ? floor(x)+epsilon : ceil(x)-epsilon;
  float zz1 = min(cellNum-1.0f, max(0.0f, a*xx1 + b)); 
  while(dx*x <= dx*x2)
  {
    //floor, ceilで値を変えるため, 外に出てしまう可能性があるので, minmaxやってる  
    float xx2 = dx>0 ?  ceil(x)-epsilon : floor(x)+epsilon;
    float zz2 = min(cellNum-1.0f, max(0.0f, a*xx2 + b));
    float z = dz>0 ? floor(zz1) : ceil(zz1);

    //衝突の可能性のあるセルを手前からプッシュ
    while( dz*z < dz*zz2)
    {
      Vector2 cell = Vector2(floor(x), floor(z));
      cells.push_back(cell);
      z += dz;
    }
    
    zz1 = zz2;    
    x += dx;
  } 

//  debugCube = cells;   //debug用, 探索するセルを描画する
  //可能性のあるセル全てと衝突判定を行う
  for(auto cell : cells)    
  {
    int index = (cell.x*cellNum + cell.y)*3*6;
    Vector3 tr1(vertexBuffer[index]  , vertexBuffer[index+1] , vertexBuffer[index+2]);
    Vector3 tr2(vertexBuffer[index+3], vertexBuffer[index+4] , vertexBuffer[index+5]);
    Vector3 tr3(vertexBuffer[index+6], vertexBuffer[index+7] , vertexBuffer[index+8]);
    Vector3 tr4(vertexBuffer[index+9], vertexBuffer[index+10], vertexBuffer[index+11]);
    Vector3 nor(normalBuffer[index]  , normalBuffer[index+1] , normalBuffer[index+2]);
        
    if(crossLineTriangle(tr1, tr2, tr3, nor, position, direction, point))     
      return true;
    
    if(crossLineTriangle(tr1, tr4, tr3, nor, position, direction, point))     
      return true;   
  }
  
  return false;  
}


//xz平面に写像した2Dフィールドで, カメラの方向ベクトルがフィールドを交差するポイントを計算
bool Field::lineCollision(const Vector3 &position, const Vector3 &direction, float &t1, float &t2) const
{  
  const Vector2 nearRight(0, 0);
  const Vector2 nearLeft(size.x, 0);
  const Vector2 farLeft(size.x, size.z);
  const Vector2 farRight(0, size.z);

  Vector2 edge[4] = {nearRight, nearLeft, farLeft, farRight };

  Vector2 pos(position.x, position.z);   //xz平面に写像したカメラ位置
  Vector2 dir(direction.x, direction.z); //マウスの方向ベクトル

  vector<float> colTime;  //交点までの時間t
  for(int i=0; i<4; i++)
  {
    float t;
    Vector2 cPos;
    //4辺との交点を計算
    if ( !crossLines2D(edge[i], edge[(i+1)%4]-edge[i], pos, dir, t, cPos) )
      continue;

    const float epsilon = 0.1;     //計算誤差用の余白部分

    //交点が後ろ,もしくは前でもフィールドの外であれば考慮しない
    if(t>0 &&
       cPos.x > -epsilon && cPos.x < size.x+epsilon &&
       cPos.y > -epsilon && cPos.y < size.z+epsilon)
      colTime.push_back(t);
  }

  if(colTime.size() == 0 || colTime.size() > 2)  
    return false;  
  
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

  const float epsilonTime = 0.1;  //誤差修正用の定数
  t1 += epsilonTime;
  t2 -= epsilonTime; 

  return true;
  
}

//pos: 移動前の位置, move: 移動量, radius: キャラクターの半径, collisionAfter:衝突判定後の位置
//返り値 : 衝突したかどうか
bool Field::collision(const Vector3 &pos, Vector3 &after, const float &radius)
{
  int xi = floor(after.x/size.x*cellNum);
  int zi = floor(after.z/size.z*cellNum);
  
  if( xi < 0 || xi>=cellNum || zi < 0 || zi>=cellNum)
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

//三角形の面上のy座標を取得
float Field::getHeight(const float &x, const float &z) const  
{
  const float xf = x/cellSize; // x/(size.x/cellNum) セルインデックスへ変換(float)
  const float zf = z/cellSize;
  
  int index = (floor(xf)*cellNum + floor(zf))*3*6;
  
  if( xf-floor(xf) + zf-floor(zf) >= 1)
    index += 9; //2枚目の三角形

  const Vector3 n(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);
  const Vector3 v(vertexBuffer[index], vertexBuffer[index+1], vertexBuffer[index+2]);
  
  return -(n.x*(x-v.x) + n.z*(z-v.z))/n.y + v.y;  
}

void Field::getNormalVectorInCell(const int &i, const int &j, Vector3 &nor1, Vector3 &nor2) const
{
  int index = cellToIndex(i,j);

  nor1 = Vector3(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);
  index += 9;  
  nor2 = Vector3(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);  
}

inline int Field::cellToIndex(const int &i, const int &j) const
{
  return (i*cellNum + j)*3*6;  
}


void Field::cellToVertices(const int &i, const int &j, Vector3 vertices[4]) const
{
  const float cellW = size.x/cellNum;
  const float cellL = size.z/cellNum;
  vertices[0] = Vector3(cellW*i    , heightMap[i  ][j  ], cellL*j    );  //near left
  vertices[1] = Vector3(cellW*(i+1), heightMap[i+1][j  ], cellL*j    );  //near right
  vertices[2] = Vector3(cellW*(i+1), heightMap[i+1][j+1], cellL*(j+1));  //far right
  vertices[3] = Vector3(cellW*i    , heightMap[i  ][j+1], cellL*(j+1));  //far left  
}

//------------------------------------------------------------//
//                 make Vertex Buffer Object                  //
//------------------------------------------------------------//
void Field::bindVBO()
{

  int vertexBufferIndex= 0;
  int texcoordBufferIndex= 0;
  int normalBufferIndex= 0;
  float texT[6], texV[6];

  Vector3 vert[6];
  Vector3 norm[2];
  
  for(int i=0; i<cellNum; i++)
  {    
    for(int j=0; j<cellNum; j++)
    {
      texT[0] = texT[3] = texT[5] = Assets::mapChip[mapchip[i][j]]->u1;
      texT[1] = texT[2] = texT[4] = Assets::mapChip[mapchip[i][j]]->u2;
      texV[0] = texV[1] = texV[3] = Assets::mapChip[mapchip[i][j]]->v1;
      texV[2] = texV[4] = texV[5] = Assets::mapChip[mapchip[i][j]]->v2;

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

  glBindBuffer(GL_ARRAY_BUFFER, 0);  
}



//============================================================//
//==================== make Terrain ====================//
//============================================================//

void Field::makeHeightMap()
{
  for(int i=0; i<=cellNum*1; i++)
    for(int j=0; j<=cellNum*1; j++)
      heightMap[i][j] = -1;

  srand(glfwGetTime());

  int x1 = cellNum*0.3;
  int z1 = cellNum*0.3;
  int x2 = cellNum*0.8;
  int z2 = cellNum*0.8;

  for( int i=x1; i<=x2; i++)
    heightMap[i][z1] = heightMap[i][z2] = 0;

  for( int j=z1; j<=z2; j++)
      heightMap[x1][j] = heightMap[x2][j] = 0;
  
//  makeMountain(x1, z1, x2, z2, 2);
}

void Field::makeMountain(const int &x1, const int &z1, const int &x2, const int &z2, const int &n)
{
  if(x1==x2 || z1==z2) return;
  int sum=0;
  int x[4] = { x1, x1, x2, x2 };  
  int z[4] = { z1, z2, z1, z2 };
  int maxHeight = 300;
  int randomness= 50;
  //四隅の平均
  for(int i=0; i<4; i++)
  {
    if(heightMap[x[i]][z[i]] == -1)
      heightMap[x[i]][z[i]] = rand()%maxHeight;
    
    sum += heightMap[x[i]][z[i]];
  } 

  //中心に, 平均+ランダム値
  int nx = floor((x1+x2)/2);
  int nz = floor((z1+z2)/2);
  heightMap[nx][nz] = sum/4.0 + rand()%randomness;
  
  if(n==0)
  {    
    merge(x1,z1,x2,z2);
  }
  else
  {    
    makeMountain(x1 ,z1, nx, nz, n-1);
    makeMountain(nx ,z1, x2, nz, n-1);
    makeMountain(x1 ,nz, nx, z2, n-1);
    makeMountain(nx ,nz, x2, z2, n-1);
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
//--------------------------------------------------------------------------------//
//-----------------------------------Create MapChip-------------------------------//
//--------------------------------------------------------------------------------//
void Field::createMapChip()
{
  for(int i=0; i<cellNum; i++)  
    for(int j=0; j<cellNum; j++)
      mapchip[i][j] = Field::Bush;

  for(int i=0; i<cellNum; i++)
  {    
    mapchip[i][i] = Field::Load;
    if(i != cellNum-1)
      mapchip[i][i+1] = Field::Load;
  }  

  auto func = [](float x)->float{return x*x*x;  };
  auto func2 = [](float x)->float{return sqrt(sqrt(x));  };

  int prev  = 0;
  int prev1 = 0;      
  for(int i=0; i<cellNum; i++)
  {
    int j = (int)( func((i+0.5)*1.0/cellNum)*cellNum);
    
    int k = (int)( func2((i+0.5)*1.0/cellNum)*cellNum);
    
    for(int l = prev; l<=k; l++)
      mapchip[i][l] = Field::Load;


    for(int l = prev1; l<=j; l++)
      mapchip[i][l] = Field::Load;

    prev1 = j;
    prev = k;    
  }
}






