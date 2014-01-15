#ifndef INFORMATION_H
#define INFORMATION_H

//グローバル定数用のクラス
class Information
{
public:
  typedef enum Buildings
  {
    LIGHTNING_TOWER,
    FREEZING_TOWER,
    BARRACK,
    FREEZIONG_TOWER,
    BuildingNum
  }Buildings;

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
  
};



  
#endif
