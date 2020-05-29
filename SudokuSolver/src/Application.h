#pragma once

#include "GLFW/glfw3.h"
#include "Board/BoardManager.h"

class Application
{
public:
   Application(GLFWwindow* pWindow, int nWidth, int nHeight);
   ~Application();

   void Run();

private:
   void DrawBackground();
   void OnUpdate();
   void OnRender();

private:
   GLFWwindow* m_pWindow;
   BoardManager m_boardManager;
   int m_nWidth;
   int m_nHeight;
};