#ifndef FIELD_H_2013_12_26
#define FIELD_H_2013_12_26

#include <math/Vector3.h>
#include "Actor.h"

class SyukatsuGame; //前方宣言
class Building;

class Field : public Actor
{
public:
    enum MapCell
  {
    Bush,
    Road,
    Start,
    Goal
  };
  static constexpr float cellSize = 30;  //1セルの大きさ
  static constexpr int cellNum = 30;     //マップサイズ

private:  
  const Vector3 position;
  const Vector3 size;

  bool mouseInRegion;  //マウスがフィールドと交差しているか
  Vector3 mousePos;    //指していればその場所  
  
  float heightMap[cellNum+1][cellNum+1];  //高さマップ
  enum MapCell mapchip[cellNum][cellNum]; //地形マップ
  Building* buildingInMap[cellNum][cellNum];  
  Building* pickedBuilding;
  
  //マップのポリゴン情報
  float vertexBuffer[cellNum*cellNum*6*3];  //頂点バッファ
  float normalBuffer[cellNum*cellNum*6*3];  //法線バッフア
  float texcoordBuffer[cellNum*cellNum*6*3];//テクスチャバッファ
  GLuint Vbold[3]; //バッファID

  //敵の軌跡
  int wavePattern;
  static constexpr int laneNum = 3;
  vector<pair<int, int>> lanes[laneNum];

  float elapsedTime;
  
  void makeHeightMap();  //マップの形状の生成
  void createMapChip();  //マップのセル状態の決定
  void bindVBO();
  void bindTexture();
  
  void merge(const int &x1, const int &z1, const int &x2, const int &z2);
  void makeMountain(const int &x1, const int &z1, const int &x2, const int &z2, const int &n);
  void interpolate(const int &x1, const int &z1, const int &x2, const int &z2);
  
  float getHeight(const float &x, const float &z) const;

  
  bool lineCollision(const Vector3 &position,const Vector3 &direction, float &t1, float &t2) const;
  bool crossLineTriangle(const Vector3 &tr1, const Vector3 &tr2, const Vector3 &tr3, const Vector3 nor,
                         const Vector3 &pos, const Vector3 &dir, Vector3 &cPos);
  
  bool getCollisionPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point);
  
  void getNormalVectorInCell(const int &i, const int &j, Vector3 &nor1, Vector3 &nor2) const;

  void cellToVertices(const int &i, const int &j, Vector3 vertices[4]) const;
  int cellToIndex(const int &i, const int &j) const ;
  void setBuildPath(const int &stX, const int &stZ, const int &glX, const int &glZ);
public:   

  Field(string name, SyukatsuGame *game);
  ~Field();

  Vector3 cellToPoint(const int &i, const int &j) const;
  pair<int, int> pointToCell(const Vector3& v) const;
  
  void render(float deltaTime);
  void update(float deltaTime);
  
  Vector3 getPosition() const { return position; }
  Vector3 getSize() const { return size; }

  enum MapCell getMapCell(const int &i, const int &j) const { return mapchip[i][j]; }

  bool collision(const Vector3 &position, Vector3 &after, const float &radius);
  void updateMousePosition(const Vector3 &position, const Vector3 &direction);
  bool getMouseCollisionPoint(Vector3 &point) const;
  bool getMouseCollisionCell(Vector2 &cell) const;

  void unPickedBuildingAll();
  void pickBuilding(const int &i, const int &j);
  bool setBuilding(Building *build, const int &i, const int &j);
  void deleteBuilding(const int &i, const int &j);
  void deleteBuilding();
  Building* getBuilding(const int &i, const int &j);
  Building* getPickedBuilding();

  bool isBuildable(const int i, const int j);
  
  
  void setLane(int lane);
  vector<pair<int, int>> getLane(int lane);

  bool laneFinished(int lane, int curdst) const;
};

#endif
