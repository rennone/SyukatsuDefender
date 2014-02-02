#include <GL/glew.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <syukatsu/syukatsu.h>
#include "TitleScene.h"
#include "Assets.h"
#include "GL/glut.h"

#include "SyukatsuDefender.h"

using namespace std;


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

   SyukatsuGame*   game = (SyukatsuGame*)glfwGetWindowUserPointer(window);
   SyukatsuInput* input = (SyukatsuInput*)game->getInput();
   input->onKey(key, action, mods);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  SyukatsuGame*   game = (SyukatsuGame*)glfwGetWindowUserPointer(window);
  SyukatsuInput* input = (SyukatsuInput*)game->getInput();
  input->onMouse(button, action, mods);
}


static void scroll_callback(GLFWwindow* window, double offsetX, double offsetY)
{
  SyukatsuGame*   game = (SyukatsuGame*)glfwGetWindowUserPointer(window);
  SyukatsuInput* input = (SyukatsuInput*)game->getInput();
  input->onScroll(offsetX, offsetY);
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
  SyukatsuGame*   game = (SyukatsuGame*)glfwGetWindowUserPointer(window);
  auto scene = game->getCurrentScene();
  scene->reshape(width, height);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glfwSetErrorCallback(error_callback);
  
  if(!glfwInit())
  {
    exit(EXIT_FAILURE);
  }
  
  bool isFullScreenMode = false;
  GLFWwindow* window;

  if(isFullScreenMode) {
    window = glfwCreateWindow(windowWidth, windowHeight, gameTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
  }
  else {
    window = glfwCreateWindow(windowWidth, windowHeight, gameTitle.c_str(), NULL, NULL);
  }

  if(!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwMakeContextCurrent(window);

  GLenum glew_error = glewInit();
  if(glew_error != GLEW_OK) {
    cout << glewGetErrorString(glew_error) << endl;
    exit(1);
  }

  SyukatsuDefender* game = new SyukatsuDefender(window);

  glfwSetWindowUserPointer(window, game); //このwindowにコールバック用にインプットを登録
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetFramebufferSizeCallback (window, resize_callback);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  
  while(!glfwWindowShouldClose(window))
  {    
    game->loop();   
  }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  exit(EXIT_SUCCESS);
}
