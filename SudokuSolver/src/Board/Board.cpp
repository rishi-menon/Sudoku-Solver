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
       ClearSelection();
   }
}

void Board::OnKey(int key)
{
    //keyboard key values
    const int nLeft = 263;
    const int nRight = 262;
    const int nUp = 265;
    const int nDown = 264;

    if (m_pCurrentCell)
    {
        //move the current selection if the arrow keys weer pressed
        //move the currnet selection with the arrow keys
        switch (key)
        {
            case nLeft:
            {
                m_nSelectedX--;
                if (m_nSelectedX < 0) m_nSelectedX = 8;
                break;
            }
            case nRight:
            {
                m_nSelectedX++;
                if (m_nSelectedX > 8) m_nSelectedX = 0;
                break;

            }
            case nUp:
            {
                m_nSelectedY++;
                if (m_nSelectedY > 8) m_nSelectedY = 0;
                break;

            }
            case nDown:
            {
                m_nSelectedY--;
                if (m_nSelectedY < 0) m_nSelectedY = 8;
                break;

            }
        }

        //insert a number into the currrently selected block if a number key was pressed
        unsigned char val = static_cast<unsigned char> (key - '0');
        if (val >= 0 && val < 10)
        {
            m_pCurrentCell->SetValue(val);
        }
    }
    else if (key >= nRight && key <= nUp)
    {
        //select the 0,0 tile by default
        m_nSelectedX = m_nSelectedY = 0;
    }

    m_pCurrentCell = &m_Cells[m_nSelectedX + m_nSelectedY * 9];

}

void Board::ClearBoard()
{
   for (int i = 0; i < 81; i++)
   {
      m_Cells[i].Clear();
   }
}