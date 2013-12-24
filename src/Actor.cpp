#include "Actor.h"

Actor::Actor(string _name, SyukatsuGame *_game):name(_name), syukatsuGame(_game)
{
  status = Actor::Action;
}

Actor::~Actor()
{
}
    
void Actor::render(float deltaTime)
{
  vector<Actor*>::iterator it = children.begin();
  while(it != children.end())
  {
    Actor* child = *it;
    if(child->status == Actor::Action || child->status == Actor::RenderOnly)
      child->render(deltaTime);
    it++;
  }
}

void Actor::update(float deltaTime)
{
  vector<Actor*>::iterator it = children.begin();
  while(it != children.end())
  {
    Actor* child = *it;
                        
    if(child->status == Actor::Action || child->status == Actor::UpdateOnly)
      child->update(deltaTime);
    it++;
  }                
}

void Actor::checkStatus()
{
  if(status == Actor::Dead)
  {
    for( auto child : children)
      child->setStatus(Actor::Dead);
  }

  for( auto child : children)          
    child->checkStatus();

  auto end_it = remove_if(children.begin(), children.end(), [](Actor* p) -> bool { return p->getStatus() == Dead; });

  for( auto it=end_it; it != children.end(); it++)
    delete (*it);
          
  children.erase(end_it, children.end());
}

void Actor::addChild(Actor* child)
{
  children.push_back(child);
}
        
Actor* Actor::searchChild(string _name) 
{
  if(this->name == _name)
    return this;

  for(auto child : children)
  {    
    auto res = child->searchChild(_name);
    if(res != NULL)
      return res;
  }
  return NULL;
}

enum Actor::ActorStatus Actor::getStatus() const
{
  return status;
}
        
void Actor::setStatus(enum ActorStatus _status)
{
  status = _status;
}
