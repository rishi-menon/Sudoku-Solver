#include "pch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/AssetManager.h"

Application::Application(GLFWwindow* pWindow, int nWidth, int nHeight) :
   m_boardManager(),
   m_pWindow (pWindow),
   m_nWidth (nWidth),
   m_nHeight (nHeight)
{
   ASSERT(m_pWindow, "Window was null");
}

Application::~Application()
{
   ASSERT(m_pWindow, "Window was null");
   if (m_pWindow) {
      glfwDestroyWindow(m_pWindow);
   }
}

void Application::Run()
{
   Renderer::Init();
   Renderer::SetMvpMatrix(0.0f, m_nWidth, 0.0f, m_nHeight);
   {
      unsigned char buf[] = { 255, 255, 255, 255 };
      Texture* whiteId = AssetManager::LoadTexture(1, 1, buf, 0);
      Texture* texId = AssetManager::LoadTexture("Assets/Image/number.png", 1);
   }
   glcall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));

   DrawBackground();
   while (!glfwWindowShouldClose(m_pWindow))
   {
      glcall(glClear(GL_COLOR_BUFFER_BIT));
      Renderer::BegineScene();

      double x, y;
      glfwGetCursorPos(m_pWindow, &x, &y);
      LOG_INFO( "Mouse Pos x:{0}, y:{1}", x,y);

      m_boardManager.OnRender();
      Renderer::EndScene();
      glfwSwapBuffers(m_pWindow);
      glfwPollEvents();
   }

   AssetManager::OnExit();
}

#include <stb_image.h>

void Application::DrawBackground()
{
   Renderer::ResetBackground();

   //bottom left corner, counter clockwise
   {
      Quad quadBackground;

      glm::vec4 colTopLeft =  { 35,35,35,255 };
      glm::vec4 colTopRight = { 35,35,35,255 };
      glm::vec4 colBotLeft =  { 35,35,35,255 };
      glm::vec4 colBotRight = { 35,35,35,255 };

      colTopLeft *= 1.0f / 255.0f;
      colTopRight *= 1.0f / 255.0f;
      colBotLeft *= 1.0f / 255.0f;
      colBotRight *= 1.0f / 255.0f;

      quadBackground.m_vertices[0] = Vertex{ {0, 0}, colBotLeft, 0, {0.0, 0.0} };

      quadBackground.m_vertices[1] = Vertex{ {m_nWidth, 0},colBotRight, 0, {1.0, 0.0} };
      
      quadBackground.m_vertices[2] = Vertex{ {m_nWidth, m_nHeight}, colTopRight, 0, {1.0, 1.0} };
      
      quadBackground.m_vertices[3] = Vertex{ {0, m_nHeight}, colTopLeft, 0, {0.0, 1.0} };
      Renderer::DrawQuad(quadBackground);
   }

   const int nPaddingBot = 4;
   const int nPaddingTop = 4;
   m_boardManager.SetBoardPos(glm::vec4{ nPaddingBot,nPaddingBot, m_nHeight- nPaddingTop, m_nHeight- nPaddingTop });
   m_boardManager.DrawBoard();
   Renderer::SetBackground();
}

void Application::OnUpdate()
{

}
void Application::OnRender()
{

}