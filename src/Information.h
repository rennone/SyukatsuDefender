#ifndef INFORMATION_H
#define INFORMATION_H

//グローバル定数用のクラス
#include <syukatsu/syukatsu.h>

namespace Information
{
  typedef enum Buildings
  {
    LIGHTNING_TOWER,
    FREEZING_TOWER,
    ARROW_TOWER,
    BUILDING_NUM,
    BARRACK
  }Buildings;

  //建物選択のショートカット
  constexpr int BuildingShortCutKeys[] =
  {
    GLFW_KEY_L,
    GLFW_KEY_F,
    GLFW_KEY_A
  };

  constexpr int DefaultRangeOfBuildings[] =
  {
    70,
    70,
    100
  };
  
  typedef enum Enemies
  {
    SOLDIER0,
    SOLDIER1,
    SOLDIER2,
    SOLDIER3,
    SOLDIER4,
    ENEMY_NUM
  }Enemies;
  
  typedef enum Buttons
  {
    DELETE_BUTTON,
    UPGRADE_BUTTON,
    BUTTON_NUM,
  } Buttons;

  typedef enum BaseValues
  {
      LIGHTNING_BASE = 100,
      FREEZING_BASE = 300,
      ARROW_BASE = 100
  } BaseValues;

  typedef enum Stages
  {
    STAGE1,
    STAGE2,
    STAGE3,
    STAGE_NUM
  } Stages;
}


namespace TextColors
{  
  class TextColor
  {
  public:
  TextColor(float _r, float _g, float _b, float _a)
    :r(_r), g(_g), b(_b), a(_a)
    {
    }
  TextColor()
    :r(1), g(1), b(1), a(1)
    {
    }

    float r,g,b,a;
  };

  typedef enum TextColors
  {
    RED,
    BLUE,
    GREEN,
    YELLOW,
    BLACK,
    WHITE,
    COLORNUM
  }TextColors;

  const TextColor RedText = TextColor(1,0,0,1);
  const TextColor BlueText = TextColor(0,0,1,1);
  const TextColor GreenText = TextColor(0,1,0,1);
  const TextColor YellowText = TextColor(1,1,0,1);
  const TextColor BlackText = TextColor(0,0,0,1);
  const TextColor WhiteText = TextColor(1,1,1,1); 
}


  
#endif
