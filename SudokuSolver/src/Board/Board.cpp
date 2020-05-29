#include "pch.h"

#include "Board.h"

Board::Board() :
   m_nextBoard (nullptr)
{

}

void Board::OnRender()
{
   for (int i = 0; i < 9; i++)
   {
      for (int j = 0; j < 9; j++)
      {
         m_Cells[i + 9 * j].OnRender(i, j);
      }
   }
}