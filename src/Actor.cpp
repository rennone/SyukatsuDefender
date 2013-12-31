#include "Actor.h"

Actor::Actor(string _name, SyukatsuGame *_game)
  :name(_name), syukatsuGame(_game)
{
  status = Actor::Action;
}

Actor::~Actor()
{
}
    
void Actor::render(float deltaTime)
{
  for( auto child : children)
  {    
    if(child->status == Actor::Action || child->status == Actor::RenderOnly)
      child->render(deltaTime);
  }
}

void Actor::update(float deltaTime)
{
  for( auto child : children)
  {                        
    if(child->status == Actor::Action || child->status == Actor::UpdateOnly)
      child->update(deltaTime);   
  }                
}

#include <iostream>
using namespace std;

void Actor::checkStatus()
{
  if(status == Actor::Dead)
  {
    for( auto child : children)
      child->setStatus(Actor::Dead);
  }

  for( auto child : children)          
    child->checkStatus();

  
  for( int i=0; i<children.size(); i++)
  {
    auto child = children[i];
    
    if(child->getStatus() == Actor::Dead)
    {      
      delete child;
      children[i] = NULL;      
    }    
  }
  
  auto end_it = remove_if(children.begin(), children.end(), [](Actor* p) -> bool { return p == NULL; });
  
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
