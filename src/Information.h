#ifndef INFORMATION_H
#define INFORMATION_H

//グローバル定数用のクラス

namespace Information
{
  typedef enum Buildings
  {
    LIGHTNING_TOWER,
    FREEZING_TOWER,
    BUILDING_NUM,
    BARRACK
  }Buildings;

  typedef enum Buttons
  {
    DELETE_BUTTON,
    UPGRADE_BUTTON,
    BUTTON_NUM,
  } Buttons;

  typedef enum BaseValues
  {
      LIGHTNING_BASE = 100,
      FREEZING_BASE = 300
  } BaseValues;
}

namespace TextColors
{ 
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
}


  
#endif
