#ifndef INFORMATION_H
#define INFORMATION_H

//グローバル定数用のクラス
#include <syukatsu/syukatsu.h>
#include <string>

namespace Information
{
  typedef enum Buildings
  {
    LIGHTNING_TOWER,
    FREEZING_TOWER,
    ARROW_TOWER,
    BUILDING_NUM,
    BARRACK,
    BREAKABLE_TREE
  }Buildings;

  const std::string BuildingName[] = {
      "LightningTower",
      "FreezingTower",
      "ArrowTower",
      "Barrack",
      "StrongHold", 
      "BreakableTree"
  };

  const std::string DescriptionOfBuildings[] =
  {
    "attacking\nmultiple enemies",
    "slowing\nsingle enemy",
    "attacking\nsingle enemy"
  };

  //建物選択のショートカット
  constexpr int BuildingShortCutKeys[] =
  {
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3
  };
  
  typedef enum Enemies
  {
    SOLDIER,
    KNIGHT,
    HEAVYARMOR,
    SOLDIER3,
    SOLDIER4,
    ENEMY_NUM
  }Enemies;

  const std::string EnemyName[] = {
      "Soldier", 
      "Knight", 
      "HeavyArmor",
      "Knight",
      "Knight"
  };
  
  typedef enum Buttons
  {
    DELETE_BUTTON,
    UPGRADE_BUTTON,
    BUTTON_NUM,
  } Buttons;

  constexpr int ButtonShortCutKeys[] =
  {
    GLFW_KEY_D,
    GLFW_KEY_U
  };
  
  typedef enum Stages
  {
    STAGE1,
    STAGE2,
    STAGE3,
    STAGE_NUM
  } Stages;

  //マップのセルの種類
  typedef enum Mapchip
  {
    Road,
    Bush
  } Mapchip;

  const float InitialMana = 100;
  const float InitialManaregen = 3.00;
  const float FireballCost = 30;

  //枠の形
  typedef enum FrameType
  {
    SOLID,
    CURVE,
    FRAME_NUM
  } FrameType;
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

  const TextColor RedText = TextColor(1,0,0,1);
  const TextColor BlueText = TextColor(0,0,1,1);
  const TextColor GreenText = TextColor(0,1,0,1);
  const TextColor YellowText = TextColor(1,1,0,1);
  const TextColor BlackText = TextColor(0,0,0,1);
  const TextColor WhiteText = TextColor(1,1,1,1); 
}


  
#endif
