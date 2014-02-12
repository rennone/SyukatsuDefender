#ifndef DEBUGGER_H_2013_11_19
#define DEBUGGER_H_2013_11_19

#include <sstream>
#include <string>
#include <map>
#include <GLFW/glfw3.h>

//Debuggerは他のファイルに依存しない形にする
class Debugger
{
public:
  //drawしたファイルの名前, タグ, 内容
  template <typename T> 
  static void drawDebugInfo(std::string place, std::string tag, T value) {
#ifdef DEBUG
      std::stringstream ss;
      ss << value;
      debugInfo[place][tag] = ss.str();
#endif
  }

  static void renderDebug(GLFWwindow *window);
  static void drawDebugCube(float x, float y, float z);
  
private:
  static std::map< std::string, std::map<std::string, std::string> > debugInfo;  //debug情報のハッシュ ( ファイル名 タグ 内容)
  static void renderString(std::string str, float x, float y);
  static void clearDebugInfo();
};

#endif
