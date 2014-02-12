#include "Debugger.h"
#include <map>
#include <GL/glut.h>
#include <iostream>

std::map< std::string, std::map<std::string, std::string> > Debugger::debugInfo;
  
void Debugger::renderDebug(GLFWwindow *window)
{

#ifdef DEBUG

  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glPushMatrix();
  glDisable(GL_LIGHTING);       
  glDisable(GL_DEPTH_TEST);
  
  int width, height;
  float ratio;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -10, 10);  
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  glColor3f(1, 1, 1);
  float x=0, y=10.3, dy=10;
    
  for( auto it = debugInfo.begin(); it != debugInfo.end(); it++)
  {			  
    //情報を書いた場所を記述
    renderString((*it).first, x, y);
    y+=dy;            
    //デバッグ情報の記述
    auto localDebugInfo = (*it).second;			
    for( auto it2 = localDebugInfo.begin(); it2 != localDebugInfo.end(); it2++, y+=dy)              
      renderString((*it2).first + " : " + (*it2).second, x, y);			  
  }
    
  glPopMatrix();
  glPopAttrib();
  clearDebugInfo();

#endif

}

void Debugger::drawDebugCube(float x, float y, float z)
{
  glPushMatrix();
  glLoadIdentity();    
  glTranslatef(x,y,z);
  glutSolidCube(20);    
  glPopMatrix();    
}

void Debugger::renderString(std::string str, float x, float y)
{
  glRasterPos2d(x,y);
  for(unsigned int i=0; i<str.size(); i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
}

void Debugger::clearDebugInfo()
{
  for(auto it=debugInfo.begin(); it!=debugInfo.end(); it++)
  {
    auto local = (*it).second;
    local.clear();
    local.erase(local.begin(), local.end());
  }
  debugInfo.erase(debugInfo.begin(), debugInfo.end());
}
