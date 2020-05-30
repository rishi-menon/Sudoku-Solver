#include "pch.h"

#include "Board.h"

Board::Board() :
   m_nextBoard (nullptr)
{

}

void Board::OnRender()
{
   if (m_pCurrentCell)
   {
      m_pCurrentCell->OnRenderSelected(m_nSelectedX, m_nSelectedY);
   }
   for (int y = 0; y < 9; y++)
   {
      for (int x = 0; x < 9; x++)
      {
         m_Cells[x + 9 * y].OnRender(x, y);
      }
   }
}

void Board::OnMouseDown(double posX, double posY)
{
   double dPercentX = MathR::GetPercent(g_rectBoard.x, g_rectBoard.z, posX);
   double dPercentY = MathR::GetPercent(g_rectBoard.y, g_rectBoard.w, posY);

   if (dPercentX >= 0 && dPercentX <= 1 && dPercentY >= 0 && dPercentY <= 1)
   {

      int nGridX = static_cast<int>(dPercentX * 9);
      int nGridY = static_cast<int>(dPercentY * 9);
      //LOG_INFO("Grid Coord: x: {0}, y: {1}", nGridX, nGridY);
      m_nSelectedX = nGridX;
      m_nSelectedY = nGridY;
      m_pCurrentCell = &m_Cells[nGridX + nGridY * 9];
   }
   else
   {
      m_nSelectedX = -1;
      m_nSelectedY = -1;
      m_pCurrentCell = nullptr;
   }
}

void Board::OnKeyDown(int key)
{
   if (m_pCurrentCell)
   {
      unsigned char val = static_cast<unsigned char> (key - '0');
      if (val >= 0 && val < 10)
      {
         m_pCurrentCell->SetValue(val);
      }
   }
}

void Board::ClearBoard()
{
   for (int i = 0; i < 81; i++)
   {
      m_Cells[i].Clear();
   }
}