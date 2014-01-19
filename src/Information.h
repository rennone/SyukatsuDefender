#ifndef INFORMATION_H
#define INFORMATION_H

//グローバル定数用のクラス

namespace Information
{
  typedef enum Buildings
  {
    LIGHTNING_TOWER,
    FREEZING_TOWER,
    BARRACK,
    BUILDING_NUM
  }Buildings;

  typedef enum Buttons
  {
    DELETE_BUTTON,
    UPGRADE_BUTTON,
    BUTTON_NUM
  } Buttons;
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
