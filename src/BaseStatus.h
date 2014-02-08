#include "Information.h"
#include <map>
#include <string>

class CharacterBaseStatus
{
  protected:
    std::map<std::string, float> status;

  public: 
    CharacterBaseStatus();

    int getMaxhp();
    float getSpeed();
    int getAttack(); 
    int getDefense(); 
    
    void load(int characterId);
};

class BuildingBaseStatus
{
  protected:
    std::map<std::string, float> status;

  public:
    BuildingBaseStatus();
    
    int getAttack();
    int getBaseValue();
    float getRangeOfEffect();
    float getAttackRate();
    
    void load(int buildingId);
};

class BaseStatus
{
  protected:
    CharacterBaseStatus **characterBaseStatus;
    BuildingBaseStatus **buildingBaseStatus;

  public:
    BaseStatus();
    void load();

    CharacterBaseStatus *getCharacterBaseStatus(int i) { return characterBaseStatus[i]; }
    BuildingBaseStatus *getBuildingBaseStatus(int i) { return buildingBaseStatus[i]; } 
};
