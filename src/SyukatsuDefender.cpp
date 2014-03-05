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
#include "SyukatsuDefender.h"
int main(int argc, char** argv)
{
  std::string str = argv[0];
  auto index = str.rfind("/");
  if(index == std::string::npos)
  {
  FilePath path;
  path.setPath("../resource/");
}
  else
  {
    std::string folder = str.substr(0, index+1) + "../resource/"; //0からindex文字とってくる.
  FilePath path;
  path.setPath(folder);
}

  SyukatsuDefender game(gameTitle.c_str(), windowWidth, windowHeight, /*is_fullscreen = */ false);

  //glew関連は,linux側でGameFrameworkの方でコンパイルできないから,そっちの設定が終わるまでここに書いておく
  GLenum glew_error = glewInit();
  if(glew_error != GLEW_OK)
  {
    cout << glewGetErrorString(glew_error) << endl;
    exit(1);
  }  
  glClearColor(0.8, 0.8, 0.8, 1.0);  //背景色の設定
  
  game.loop();    //ゲーム開始
}




/*
  //テンプレートなのでSyukatsuGameに移動した
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
