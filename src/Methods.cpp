#include "Methods.h"

namespace Methods
{
  bool crossLine(const Vector2 &p1, const Vector2 &d1, const Vector2 &p2, const Vector2 &d2, Vector2 &colPos, float &t2)
  {
    //平行なら, 交差しない
    if( d1.x == d2.x && d1.y == d2.y)
      return false;

    // 連立方程式
    // p1.x + d1.x*t1 = p2.x + d2.x*t2
    // p1.y + d1.y*t1 = p2.y + d2.y*t2  
    // を解いて t2を求める
    t2 = (d1.y*p1.x - d1.x*p1.y + d1.x*p2.y - d1.y*p2.x) / (d1.y*d2.x - d1.x*d2.y);
    colPos = p2 + d2*t2;
  return true;
  }

  // 三角形の法線方向を求める
  // v1->v2->v3 回りで求める
  Vector3 normalVectorOfTriangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
  {
    const Vector3 A = v2-v1;
    const Vector3 B = v3-v2;
    Vector3 norm = A.cross(B);
  
    norm.normalize();
    return norm;
  }

//  bool croosLineAndTriangle(const Vector3 &tr1, const Vector3 &tr2, const Vector3 &tr3, )
}

/*
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
*/
