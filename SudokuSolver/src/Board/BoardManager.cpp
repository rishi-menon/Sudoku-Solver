
#include "pch.h"
#include "BoardManager.h"
#include "Renderer/Renderer.h"

glm::vec4 g_rectBoard;

BoardManager::BoardManager() :
   m_bSolving (false)
{
   m_pBoard = new Board;
}
BoardManager::~BoardManager()
{
   while (m_pBoard)
   {
      Board* pBoard = m_pBoard->GetNext();
      delete m_pBoard;
      m_pBoard = pBoard;
   }
}

void BoardManager::OnMouseDown(double posX, double posY)
{
   if (m_pBoard)
   {
      m_pBoard->OnMouseDown(posX, posY);
   }
   else { LOG_WARN("Warning: Current board is null"); }
}
void BoardManager::OnKey(int key)
{
   const int nSolvingKey = 'S';
   const int nClearAllKey = 'C';
   if (m_pBoard)
   {
      //Start solving here
      if (!m_bSolving)
      {
         m_pBoard->OnKey(key);
         if (key == nSolvingKey)
         {
            m_bSolving = true;
            m_pBoard->ClearSelection();
         }
      }
      if (key == nClearAllKey)
      {
         Clear();
         m_bSolving = false;
      }
   }
   else { LOG_WARN("Warning: Current board is null"); }
}

void BoardManager::Clear()
{
   if (!m_pBoard) m_pBoard = new Board();
   else
   {
      Board* pNext = m_pBoard->GetNext();
      while (pNext)
      {
         Board* pBoard = pNext->GetNext();
         delete pNext;
         pNext = pBoard;
      }

      m_pBoard->SetNext(nullptr);
      m_pBoard->ClearBoard();
   }
}
void BoardManager::DrawBoard()
{
   //properties
   const glm::vec4 colMajor = { 0.0, 0.0, 0.0, 1.0 };
   const glm::vec4 colMinor = { 0.2,0.2,0.2,0.7 };
   const float nThicknessMinor = 3;
   const float nThicknessMajor = 5;

   //helper variables
   const float width  = (g_rectBoard.z - g_rectBoard.x);
   const float height = (g_rectBoard.w - g_rectBoard.y);
   const float startingX = g_rectBoard.x;
   const float startingY = g_rectBoard.y;
   
   float x = g_rectBoard.x + width / 9.0f;
   float y = g_rectBoard.y + height / 9.0f;
   for (int i = 1; i < 10; i++)
   {
      if (i % 3 != 0)
      {
         Renderer::DrawQuad(startingX, y, width, nThicknessMinor, colMinor);//horizontal
         Renderer::DrawQuad(x, startingY, nThicknessMinor, height, colMinor);//vertical
      }

      x += width/9.0f;
      y += height/9.0f;
   }

   //draw major gridlines ON TOP of the minor ones
   x = g_rectBoard.x;
   y = g_rectBoard.y;
   for (int i = 0; i < 4; i++)
   {
      Renderer::DrawQuad(startingX, y, width, nThicknessMajor, colMajor);//horizontal
      Renderer::DrawQuad(x, startingY, nThicknessMajor, height, colMajor);//vertical
      x += width / 3.0f;
      y += height / 3.0f;
   }
}

void BoardManager::OnRender()
{
   if (m_pBoard)
   {
      m_pBoard->OnRender();
   }
   else { LOG_WARN("Warning: Current board is null"); }
}

void BoardManager::Update()
{
   if (!m_bSolving) return;

   //Start solving here



}