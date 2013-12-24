#ifndef _ACTOR_H_2013_11_19
#define _ACTOR_H_2013_11_19

#include <vector>
#include <string>
#include <algorithm>
#include <syukatsu/SyukatsuGame.h>

using namespace std;

class Actor
{
public:
  enum ActorStatus
  {
    Action,
    UpdateOnly,
    RenderOnly,
    NoUse,
    Dead
  };  
  
protected:
  SyukatsuGame* const syukatsuGame;
  const string name;
  vector<Actor*> children;
  enum ActorStatus status;
public:
  Actor(std::string _name, SyukatsuGame *_game);
  virtual ~Actor();    
  virtual void render(float deltaTime);
  virtual void update(float deltaTime);
  virtual void checkStatus();
  virtual void addChild(Actor* child);        
  Actor* searchChild(std::string _name);
  enum ActorStatus getStatus() const;
  void setStatus(enum ActorStatus _status);
};

#endif
