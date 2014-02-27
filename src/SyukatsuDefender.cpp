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

int main(int argc, char** argv)
{
  //glutを使う
  glutInit(&argc, argv);  
  auto game = new SyukatsuDefender(gameTitle.c_str(), windowWidth, windowHeight, /*is_fullscreen = */ false);
  //glewを使う
  GLenum glew_error = glewInit();
  if(glew_error != GLEW_OK)
  {
    cout << glewGetErrorString(glew_error) << endl;
    exit(1);
  }  
  //背景色の設定
  glClearColor(0.8, 0.8, 0.8, 1.0);

  //ゲーム開始
  game->loop();
  
  /*
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
  glClearColor(0.8, 0.8, 0.8, 1.0);
  
  while(!glfwWindowShouldClose(window))
  {    
    game->loop();   
  }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  exit(EXIT_SUCCESS);
  */
}
