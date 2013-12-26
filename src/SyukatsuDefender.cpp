#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <syukatsu/syukatsu.h>
#include "TitleScene.h"
#include "Assets.h"
#include "GL/glut.h"

using namespace std;

//mainで書き換えるのはこのクラスだけ
class SyukatsuDefender:public SyukatsuGame
{
public:
  SyukatsuDefender(GLFWwindow* window)
    :SyukatsuGame(window)
  {
    Assets::load();    
    scene = getStartScene();
  }

  Scene* getStartScene()
  {
    return new TitleScene(this);    
  }
  
};

//------------------------------------------------------------//
//こっから下は基本固定
//------------------------------------------------------------//
static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{  
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  //デバッグ用 F1で強制終了
  if (key == GLFW_KEY_F1 && action == GLFW_PRESS)    exit(2);

  ((SyukatsuInput*)glfwGetWindowUserPointer(window))->onKey(key, action, mods);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  ((SyukatsuInput*)glfwGetWindowUserPointer(window))->onMouse(button, action, mods);
}

#include <unistd.h>
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glfwSetErrorCallback(error_callback);
  
  if(!glfwInit())
  {
    exit(EXIT_FAILURE);
  }

//  GLFWwindow* window = glfwCreateWindow(640, 480, "example", glfwGetPrimaryMonitor(), NULL);  //フルスクリーン
  GLFWwindow* window = glfwCreateWindow(640, 480, "shoot", NULL, NULL);
  if(!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwMakeContextCurrent(window);
  SyukatsuDefender* game = new SyukatsuDefender(window);
  
  glfwSetWindowUserPointer(window, game->getInput()); //このwindowにコールバック用にインプットを登録
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_callback);
  
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  while(!glfwWindowShouldClose(window))
  {    
    game->loop();   
  }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  exit(EXIT_SUCCESS);
}
