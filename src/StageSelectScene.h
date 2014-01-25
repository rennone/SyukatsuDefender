#ifndef STAGE_SELECT_SCENE_H
#define STAGE_SELECT_SCENE_H

#include <syukatsu/syukatsu.h>
#include "Information.h"
#include "Icon.h"
class MouseMoveCamera;
class Field;


using namespace Information;
class StageSelectScene : public SyukatsuScene
{
Icon *icons[STAGE_NUM];
  Camera3D *camera;
  Camera2D *menuCamera;
SpriteBatcher *batcher;
float MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT;
Field *field;
int select;
public:
  StageSelectScene(SyukatsuGame *game);
  ~StageSelectScene();
void update(float deltaTime);
void render(float deltaTime);

void dispose()
{
};

void reshape(int width, int height)
{
};
};

#endif
