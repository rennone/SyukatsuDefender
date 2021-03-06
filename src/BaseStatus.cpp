#include "BaseStatus.h"
#include <iostream>
#include <string>
#include <fstream>

const std::string statusDir = "../resource/BaseStatus/";
const std::string statusExtension = ".txt";

CharacterBaseStatus::CharacterBaseStatus()
{
}

void CharacterBaseStatus::load(int characterId) {
  std::string fileName = statusDir + Information::EnemyName[characterId] + statusExtension;

  std::ifstream ifs(fileName);

  if(!ifs) {
    std::cout << "cannot open file : " << fileName << std::endl;
  }

  while(ifs) {
    std::string statName;
    float value;
    
    ifs >> statName >> value;
    status[statName] = value;
  }
}


int CharacterBaseStatus::getMaxhp() 
{
  const std::string param = "maxhp";
  if(status.count(param) == 0) {
    std::cout << param <<  "is not setup" << std::endl;
  }

  return (int)status[param];
}

float CharacterBaseStatus::getSpeed() 
{
  const std::string param = "speed";
  if(status.count(param) == 0) {
    std::cout << param <<  "is not setup" << std::endl;
  }

  return status[param];
}

int CharacterBaseStatus::getAttack() 
{
  const std::string param = "attack";
  if(status.count(param) == 0) {
    std::cout << param <<  "is not setup" << std::endl;
  }

  return (int)status[param];
}

int CharacterBaseStatus::getDefense() 
{
  const std::string param = "defense";
  if(status.count(param) == 0) {
    std::cout << param <<  "is not setup" << std::endl;
  }

  return (int)status[param];
}

BuildingBaseStatus::BuildingBaseStatus()
{
}

void BuildingBaseStatus::load(int buildingId) {
  std::string fileName = statusDir + Information::BuildingName[buildingId] + statusExtension;
  std::ifstream ifs(fileName);

  if(!ifs) {
    std::cout << "cannot open file : " << fileName << endl;
  }

  while(ifs) {
    std::string statName;
    float value;
    
    ifs >> statName >> value;
    status[statName] = value;
  }
}


int BuildingBaseStatus::getAttack() 
{
  const std::string param = "attack";
  if(status.count(param) == 0) {
    cout << param <<  "is not setup" << endl;
  }

  return (int)status[param];
}

int BuildingBaseStatus::getBaseValue() 
{
  const std::string param = "baseValue";
  if(status.count(param) == 0) {
    cout << param <<  "is not setup" << endl;
  }

  return (int)status[param];
}

float BuildingBaseStatus::getRangeOfEffect() 
{
  const std::string param = "rangeOfEffect";
  if(status.count(param) == 0) {
    cout << param <<  "is not setup" << endl;
  }

  return status[param];
}

float BuildingBaseStatus::getAttackRate() 
{
  const std::string param = "attackRate";
  if(status.count(param) == 0) {
    cout << param <<  "is not setup" << endl;
  }

  return status[param];
}

BaseStatus::BaseStatus()
{
  characterBaseStatus = new CharacterBaseStatus*[Information::Enemies::ENEMY_NUM];
  for(int i = 0; i < Information::Enemies::ENEMY_NUM; ++i) {
    characterBaseStatus[i] = new CharacterBaseStatus;
  }

  buildingBaseStatus = new BuildingBaseStatus*[Information::Buildings::BUILDING_NUM];
  for(int i = 0; i < Information::Buildings::BUILDING_NUM; ++i) {
    buildingBaseStatus[i] = new BuildingBaseStatus;
  }

  load();
}

void BaseStatus::load()
{
  for(int i = 0; i < Information::Enemies::ENEMY_NUM; ++i) {
    characterBaseStatus[i]->load(i);
  }

  for(int i = 0; i < Information::Buildings::BUILDING_NUM; ++i) {
    buildingBaseStatus[i]->load(i);
  }
}
