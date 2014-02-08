#include <GL/glew.h>
#include "Field.h"
#include "Assets.h"
#include "Building.h"

#include "SimpleObjectFactory.h"
#include "Debugger.h"
//#include <syukatsu/GL/glut.h>
//#include <syukatsu/syukatsu.h>
#include "MessageManager.h"
#include "PlayScene.h"
#include "Methods.h"
#include <sstream>
#include <string.h>
#include <iostream>
using namespace std;

//0~1を受け取り 0~1を返す関数郡(ただし, 0の時は0 1の時は1を返す)
static float pattern0(float p) { return p; }
static float pattern1(float p) { return p*p*p; }
static float pattern2(float p) { return sin(p*M_PI/2); }

static float pattern3(float p) { return sqrt(p); }
static float pattern4(float p) { return 1-pow(cos(p*M_PI/2), 1.0/4.0); }
static float pattern5(float p) { return p*pow(sin(p*M_PI/2), 3); }

static float pattern6(float p) { return pow(p, 0.2); }
static float pattern7(float p) { return 2.0/(1+exp(-p)) - 1; }
static float pattern8(float p) { return p*pow(sin(p*5*M_PI/2), 4) + (1-p)*cos((1-p)*M_PI/2); }

static float ( *patterns[] )(float) =
{
  pattern0,  pattern1,  pattern2,
  pattern3,  pattern4,  pattern5,
  pattern6,  pattern7,  pattern8,
};

static const int debug_pattern = 2;
static constexpr int waveNum = 3;
static vector<Vector2> debugCube;

bool Field::crossLineTriangle(const Vector3 &tr1, const Vector3 &tr2, const Vector3 &tr3, const Vector3 nor,
                              const Vector3 &pos, const Vector3 &dir, Vector3 &cPos)
{
  //平面との衝突点
  /*
    
   */
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

//------------------------------変換メソッド------------------------------//

//セルの端点を取得
void Field::cellToVertices(const int &i, const int &j, Vector3 vertices[4]) const
{
  vertices[0] = Vector3(cellSize*i    , heightMap[i  ][j  ], cellSize*j    );  //near left
  vertices[1] = Vector3(cellSize*(i+1), heightMap[i+1][j  ], cellSize*j    );  //near right
  vertices[2] = Vector3(cellSize*(i+1), heightMap[i+1][j+1], cellSize*(j+1));  //far right
  vertices[3] = Vector3(cellSize*i    , heightMap[i  ][j+1], cellSize*(j+1));  //far left  
}

//------------------------------ コンストラクタ ------------------------------//
Field::Field(string name, SyukatsuGame *game)
  :Actor(name, game)
  ,position(Vector3(0,0,0))
  ,size(Vector3(cellNum*cellSize, 0, cellNum*cellSize))
  ,mouseInRegion(false)
  ,pickedBuilding(NULL)
  ,elapsedTime(0)
{
  //初期化
  for(int i=0; i<cellNum; i++)
    for(int j=0; j<cellNum; j++)
    {
      buildingInMap[i][j] = NULL;
      mapchip[i][j] = Bush;
    }
  
  setLane(0);
  makeHeightMap(); //高さマップの自動生成
  bindVBO(); //フィールドの頂点情報をVBO化

  baseStatus = new BaseStatus;
}

///------------------------------ デストラクタ ------------------------------//
Field::~Field()
{  
}

//------------------------------アップデード------------------------------//
void Field::update(float deltaTime)
{
  elapsedTime += deltaTime;
  Debugger::drawDebugInfo("Field.cpp", "picked", pickedBuilding==NULL ? "null" : "notNull");
}

//------------------------------render------------------------------//
void Field::render(float deltaTime)
{  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);  
  glDisable(GL_LIGHTING);
  Assets::fieldAtlas->bind();  
  renderMap();   //操作範囲内のマップを描画
  renderField(); //マップの外のスカイボックスを描画
  glPopAttrib();
}

void Field::renderField()
{
  //フィールドの外の描画
  const float skySize = size.x*4.0;
  drawTexture(Vector3(size.x/2,-3,size.z/2), Vector3(0,1,0), size.x*1.05, Assets::regionFrame);
  drawTexture(Vector3(size.x/2,-5,size.z/2), Vector3(0,1,0), skySize   , Assets::mapChip[1]);

  //4面のスカイボックスの描画
  float dx[] = { -skySize/2+size.x/2, skySize/2+size.x/2,       size.x/2, size.x/2};
  float dz[] = { size.x/2,       size.x/2, skySize/2+size.x/2, -skySize/2+size.x/2};
  float nx[] = { 1,      -1,       0, 0};
  float nz[] = { 0,       0,      -1, 1};
  for(int i=0; i<4; i++)
    drawTexture( Vector3(dx[i], skySize/2-5, dz[i]),
               Vector3(nx[i],0,nz[i]), skySize, Assets::skybox[i] );
}

void Field::renderMap()
{
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
 
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);  
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);//必要
}

//マウスが指しているフィールドの位置を取得
bool Field::getMouseCollisionCell(Vector2 &cell) const
{
  if(!mouseInRegion)
    return false;

  cell.set(mouseCell.first, mouseCell.second);
  
  return true;
  
  /*
  //とりあえず, どんなセルでも指せる
  Vector3 nor1, nor2;
  getNormalVectorInCell(cell.x, cell., nor1, nor2);    
  if(nor1.dot(Vector3(0,1,0)) < 0.9 || nor2.dot(Vector3(0,1,0)) < 0.9)
   return false;
  */
}

//選択を外す
void Field::unPickedBuildingAll()
{
  if ( pickedBuilding != NULL )
    pickedBuilding->setPicked(false);
  
  pickedBuilding = NULL;
}

//(i,j)セルの建物を選択
void Field::pickBuilding(const int &i, const int &j) 
{
  if ( pickedBuilding != NULL )
    pickedBuilding->setPicked(false);
  
  pickedBuilding = NULL;
  
  if(buildingInMap[i][j] == NULL)
    return;

  pickedBuilding = buildingInMap[i][j];
  pickedBuilding->setPicked(true);
}

bool Field::setBuilding(Building *build, const int &i, const int &j)
{
  if( !isBuildable(i, j) || build == NULL)
    return false;

  buildingInMap[i][j] = build;  
  return true;
}

//建設可能なセルかチェック
bool Field::isBuildable(const int i, const int j)
{
  if(i < 0 || j < 0 || i >= cellNum || j >= cellNum)
    return false;

  //通路には置けない
  if( mapchip[i][j] == Information::Road) 
    return false;  
  
  //すでに建物があれば置けない
  if(buildingInMap[i][j] != NULL) 
    return false;  

  return true;
}

//指定セルの建物を削除
void Field::deleteBuilding(const int &i, const int &j)
{
  if(i<0 || j<0 || i>=cellNum || j>= cellNum)
    return;

  buildingInMap[i][j]->setStatus(Actor::Dead);
  buildingInMap[i][j] = NULL;
}

//選択している建物を消す
void Field::deleteBuilding()
{
  if ( pickedBuilding == NULL )
    return;
  
  pair<int, int> cell = pointToCell( pickedBuilding->getPosition());
  
  deleteBuilding(cell.first, cell.second);
  pickedBuilding = NULL;
}

//選択している建物を取得
Building* Field::getPickedBuilding() 
{
  return pickedBuilding;
}

//指定したセルの建物を取得
Building* Field::getBuilding(const int &i, const int &j)
{
  if(i < 0 || j < 0 || i >= cellNum || j >= cellNum) 
    return NULL;  

  return buildingInMap[i][j];
}

//マウスが指しているフィールドの位置を取得
bool Field::getMouseCollisionPoint(Vector3 &point) const
{
  if(!mouseInRegion)
    return false;
  
  point = mousePoint;
  return true;
}

//マウスが指しているフィールドの位置を計算(毎フレームの最初に呼び出す)
void Field::updateMousePosition(const Vector3 &position, const Vector3 &direction)
{  
  //mouseInRegion = getCollisionPoint(position, direction, mousePoint);

  //フィールドは, 平面と仮定し衝突判定を簡単にする
  if ( direction.y == 0 )
  {
    mouseInRegion = false;
    return;
  }
  
  const float t = -position.y/direction.y;

  Vector3 ground = position + direction*t;
  if ( t<=0 || ground.x <= 0 || size.x <= ground.x || ground.z <= 0 || size.z <= ground.z )
  {
    mouseInRegion = false;
    return;
  }
  
  mousePoint = ground;
  mouseCell  = pointToCell(ground);
  mouseInRegion = true;
}

//pos: 移動前の位置, move: 移動量, radius: キャラクターの半径, collisionAfter:衝突判定後の位置
//返り値 : 衝突したかどうか
bool Field::collision(const Vector3 &pos, Vector3 &after, const float &radius)
{
  if ( !inRegion(after) )
  {
    after = pos;
    return true;
  }
  
  after.y = getHeight(after.x, after.z) + radius;
  return false; 
}

//三角形の面上のy座標を取得
float Field::getHeight(const float &x, const float &z) const  
{  
  // float単位のセルインデックスへ変換
  const float xf = x/cellSize;
  const float zf = z/cellSize;
  
  int index = cellToIndex(floor(xf), floor(zf));
  
  if( xf-floor(xf) + zf-floor(zf) >= 1)
    index += 9; //2枚目の三角形

  const Vector3 n(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);
  const Vector3 v(vertexBuffer[index], vertexBuffer[index+1], vertexBuffer[index+2]);

  //todo
  // (x-v.x, z-v.z)を法線をnとする面に射影
  return -(n.x*(x-v.x) + n.z*(z-v.z))/n.y + v.y;  
}

//セルの法線を求める. 使う前に, i, jの値のチェックが必要
void Field::getNormalVectorInCell(const int &i, const int &j, Vector3 &nor1, Vector3 &nor2) const
{
  int index = cellToIndex(i,j);

  nor1 = Vector3(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);
  index += 9;  
  nor2 = Vector3(normalBuffer[index], normalBuffer[index+1], normalBuffer[index+2]);  
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

      //セルの端点の座標を求める.
      vert[0] = Vector3(cellSize*i    , heightMap[i  ][j  ], cellSize*j    );  //near left
      vert[1] = Vector3(cellSize*(i+1), heightMap[i+1][j  ], cellSize*j    );  //near right
      vert[2] = Vector3(cellSize*(i+1), heightMap[i+1][j+1], cellSize*(j+1));  //far right
      vert[3] = Vector3(cellSize*i    , heightMap[i  ][j+1], cellSize*(j+1));  //far left  

      vert[5] = vert[3];
      vert[3] = vert[0];
      vert[4] = vert[2];
      
      norm[0] = Methods::normalVectorOfTriangle(vert[0], vert[1], vert[2]);
      norm[1] = Methods::normalVectorOfTriangle(vert[3], vert[4], vert[5]);
      
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

//--------------------------------------------------------------------------------//
//-----------------------------------Create MapChip-------------------------------//
//--------------------------------------------------------------------------------//
void Field::setBuildPath(const int &stX, const int &stZ, const int &glX, const int &glZ)
{
  int x = stX;
  int z = stZ; 

  int dx = glX - stX;
  int dz = glZ - stZ;

  int stepX = (dx>0) - (dx<0);
  int stepZ = (dz>0) - (dz<0);

  mapchip[x][z] = Road;

  dx = abs(dx);
  dz = abs(dz);
  
  if ( dx < dz)
  {
    float tmp = 2*dx - dz;
    while(z != glZ)
    {
      if(tmp >= 0)
      {        
        mapchip[x][z+stepZ] = Road;
        x += stepX;
        tmp -= dz;     
      }
      z += stepZ;
      tmp += dx;
      mapchip[x][z] = Road;
    }
  }  
  else
  {
    float tmp = 2*dz - dx;
    while( x != glX)
    {
      if( tmp >= 0 )
      {
        mapchip[x+stepX][z] = Road;
        z += stepZ;
        tmp -= dx;
      }
      x += stepX;
      tmp += dz;
      mapchip[x][z] = Road;      
    }
  }  
}

void Field::createMapChip()
{
  for(int i=0; i<cellNum; i++)
    for(int j=0; j<cellNum; j++)
      mapchip[i][j] = Bush;
  
  int patternNum = sizeof(patterns)/sizeof(patterns[0]);
  int split = 20;

  for(int k=0; k<laneNum; k++)
  {
    int prevX = cellNum-1, prevY = cellNum-1;
    for(int i=0; i<=split; i++)
    {
      float p = 1.0 - i*1.0/split; //ゴールが(0,0)なので逆にしてる

      int pattern = (wavePattern*laneNum + k)%patternNum;
      //1になった時 cellNumになるので
      int x  = min(cellNum-1.0f, p*cellNum);
      //int y  = min(cellNum-1.0f, patterns[debug_pattern](p)*cellNum);
      int y  = min(cellNum-1.0f, patterns[pattern](p)*cellNum);
      //cout << x << "," << y << endl;
      setBuildPath(prevX,prevY, x, y);
      prevX = x;
      prevY = y;
    }
  }
}

void Field::setLane(int wave)
{
  if ( wave < 0 || wave >= Information::STAGE_NUM)
    wavePattern = rand()%Information::STAGE_NUM;
  else
    wavePattern = wave;

  for(int i=0; i<laneNum; i++)
    lanes[i].clear();

  createMapChip();
  const int split = 20;
  for(int k=0; k<laneNum; k++)
    for(int i=0; i<=split; i++)
    {
      int pattern = wavePattern*laneNum + k;
      float p = 1 - i*1.0/split; //ゴールが(0,0)なので逆にしてる    
      int x  = min(cellNum-1.0f, p*cellNum);       //切り捨てする事で, 最後は0,0になるようにしてる, 最初はcellNum-1, cellNum-1になるようにしてる    
      int y1 = min(cellNum-1.0f,patterns[pattern](p)*cellNum);
   
      lanes[k].push_back(make_pair(x, y1));
    }

  bindTexture();
}

//3つのレーンうちランダムに取得
vector< pair<int, int> > Field::getLane(int lane)
{
  if(lane < 0 || lane >= laneNum)
    return lanes[rand()%laneNum];  

  return lanes[lane];
}

//Textureをバインドする
void Field::bindTexture()
{
  int texcoordBufferIndex= 0;
  float texT[6], texV[6];
  
  for(int i=0; i<cellNum; i++)
  {    
    for(int j=0; j<cellNum; j++)
    {
      texT[0] = texT[3] = texT[5] = Assets::mapChip[mapchip[i][j]]->u1;
      texT[1] = texT[2] = texT[4] = Assets::mapChip[mapchip[i][j]]->u2;
      texV[0] = texV[1] = texV[3] = Assets::mapChip[mapchip[i][j]]->v1;
      texV[2] = texV[4] = texV[5] = Assets::mapChip[mapchip[i][j]]->v2;
      
      for(int k=0; k<6; k++)
      {
        texcoordBuffer[texcoordBufferIndex++] = texT[k];
        texcoordBuffer[texcoordBufferIndex++] = texV[k];
      }
    }
  } 
  //テクスチャ
  glBindBuffer(GL_ARRAY_BUFFER, Vbold[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordBuffer), texcoordBuffer, GL_STREAM_DRAW);
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



//------------------------------------------------------------//
//   フィールドの様々な形状に対応した当たり判定
//------------------------------------------------------------//
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


//xz平面に写像した2Dフィールドで, カメラの方向ベクトルがフィールドと交差するポイントを計算
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
    if ( !Methods::crossLine(edge[i], edge[(i+1)%4]-edge[i], pos, dir,cPos, t) )
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
