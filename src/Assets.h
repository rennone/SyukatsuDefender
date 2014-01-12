#ifndef ASSETS_H_2013_12_22
#define ASSETS_H_2013_12_22

#include <Texture.h>
#include <Model.h>
#include <Font.h>
#include <string>
using namespace std;

class Assets
{
public:
  static string resourcePath;
  static Texture   *textureAtlas;
  static TextureRegion *virus;
  static TextureRegion *player;
  static TextureRegion *start;
  static TextureRegion *title;
  static TextureRegion *bullet;
  static TextureRegion *frame;
  static TextureRegion *lightningTowerIcon;
  static TextureRegion *barrackIcon;
  static TextureRegion *focusedFrame;
  static TextureRegion *highLight;  
  static TextureRegion *background;
  static TextureRegion *mapChip[4];
  static TextureRegion *explosion;
  static Model    *simpleModel;
  static Font     *mincho;  
  static void load();
  static void dispose();
private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
