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
  static TextureRegion *ground;
  static TextureRegion *frame;
  static TextureRegion *focusedFrame;
  static TextureRegion *background;
//  static Model    *simpleModel;
  static Font     *mincho;  
  static void load();
  static void dispose();
private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
