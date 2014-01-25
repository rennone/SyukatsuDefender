#ifndef WIDGET_H
#define WIDGET_H

class SpriteBatcher;

template<class T>
class Widget
{
  T content;

public:
Widget(T _content):content(_content) { }
  ~Widget() {  }    
  
  virtual void setContent(T _content)
  {
    content = _content;    
  }
  
  virtual T getContent() const
  {
    return content;    
  }

  virtual void render(bool focus, SpriteBatcher *batcher) = 0;
  
};

#endif
