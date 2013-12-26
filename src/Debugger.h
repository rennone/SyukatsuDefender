#ifndef DEBUGGER_H_2013_11_19
#define DEBUGGER_H_2013_11_19

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

//Debuggerは他のファイルに依存しない形にする
class Debugger
{
public:
  //drawしたファイルの名前, タグ, 内容
  template<typename T> static void drawDebugInfo(std::string place, std::string tag, T value)
  {
    std::stringstream ss;
    ss << value;
    debugInfo[place][tag] = ss.str();
  }

  //画面サイズ
  static void renderDebug(GLFWwindow *window)
  {
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    int width, height;
    float ratio;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -10, 10);
  
    glMatrixMode(GL_MODELVIEW);

    
    glDisable(GL_LIGHTING);
    glPushMatrix();
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
    glPopAttrib();
    glPopMatrix();
    clearDegubInfo();
  }
  
private:
  //degub情報のハッシュ ( ファイル名 タグ 内容)
  static std::map< std::string, std::map<std::string, std::string>> debugInfo;

  static void renderString(std::string str, float x, float y)
  {
    glRasterPos2d(x,y);
    for(unsigned int i=0; i<str.size(); i++)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
  }

  static void clearDegubInfo()
  {
    for(auto it=debugInfo.begin(); it!=debugInfo.end(); it++)
    {
      auto local = (*it).second;
      local.clear();
      local.erase(local.begin(), local.end());
    }
    debugInfo.erase(debugInfo.begin(), debugInfo.end());
  }
};

#endif
