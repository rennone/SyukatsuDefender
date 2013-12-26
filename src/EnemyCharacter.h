#ifndef ENEMY_CHARACTER_H_2013_12_26
#define ENEMY_CHARACTER_H_2013_12_26

class EnemyCharacter: public Character
{
public:
  EnemyCharacter(string name, syukatsuGame *game)
    :Character(name, game)
  {
    
  }
  ~EnemyCharacter() {  }    
};
#endif
