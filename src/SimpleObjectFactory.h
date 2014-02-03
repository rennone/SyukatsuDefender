#ifndef SIMPLE_OBJECT_FACTORY_H_2013_12_20
#define SIMPLE_OBJECT_FACTORY_H_2013_12_20

#include <math/Vector3.h>
#include <syukatsu/syukatsu.h>

extern void drawGrid(const float xNum,const float zNum,const float size);
extern void drawTexCube(const float size,const int tex,const bool reverse);

//テクスチャの描画
//位置, 法線, 正方形のサイズ, テクスチャ
extern void drawTexture(const Vector3 &position, const Vector3 &normal, const float size, const TextureRegion *region);

extern void drawTexture(const Vector3 &position, const Vector3 &normal, const Vector2 &size, const TextureRegion *region);

extern void drawAxis();

#endif
