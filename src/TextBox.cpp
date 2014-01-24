#include "TextBox.h"
#include <syukatsu/syukatsu.h>
#include "Assets.h"
TextBox::TextBox(string text)
  :Widget(text)   
{
}

TextBox::TextBox(string text, Vector2 lowerLeft, Vector2 size, int _fontSize)
  :Widget(text)
  ,fontSize(_fontSize)
{
  region.lowerLeft.set(lowerLeft);
  region.size.set(size);  
}

void TextBox::render(bool focus, SpriteBatcher *batcher)
{
/*
  batcher->drawSprite(region.lowerLeft.x + region.size.x/2, region.lowerLeft.y + region.size.y/2,
                      region.size.x, region.size.y, focus ? Assets::focusedFrame : Assets::frame);
*/
  glPushMatrix();
  
  glTranslatef(region.lowerLeft.x, region.lowerLeft.y+region.size.y/2, 0);  
  Assets::mincho->setSize(fontSize);
  
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_TEXTURE_2D);  
  Assets::mincho->render(getContent());
  glPopAttrib();
  
  glPopMatrix();

}
