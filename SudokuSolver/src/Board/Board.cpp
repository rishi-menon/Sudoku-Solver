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
void Board::ResetPossibleValues()
{
    
    for (int i = 0; i < 81; i++)
    {
        m_Cells[i].SetPossibleL(0x01FF);
    }
}
void Board::LoadBoard(unsigned char data[81])
{
    for (int x = 0; x < 81; x++)
    {
        m_Cells[x].Clear();
        char c = data[x];
        if (c >= 0 || c <= 9)
        {
            m_Cells[x].SetValue(c);
        }
    }
}
bool Board::IsSolved() const
{
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (m_Cells[x + 9 * y].GetValue() == 0) return false;
        }
    }
    return true;
}

bool Board::CheckContradiction() const
{
    auto ResetArray = [](bool arr[])
    {
#if 0
        for (int i = 0; i < 9; i++)
        {
            arr[i] = 0;
        }
#else
        memset(arr, 0, 10);
#endif
    };

    bool seen[10];
    
    //check contradictions along columns
    for (int x = 0; x < 9; x++)
    {
        ResetArray(seen);
        for (int y = 0; y < 9; y++)
        {
            unsigned char c = m_Cells[x + y * 9].GetValue();
            if (c > 0)
            {
                ASSERT(c <= 9, "Invalid value of cell");
                if (seen[c])    return true;
                seen[c] = true;
            }
        }
    }

    //check contradictions along rows
    for (int y = 0; y < 9; y++)
    {
        ResetArray(seen);
        for (int x = 0; x < 9; x++)
        {
            unsigned char c = m_Cells[x + y * 9].GetValue();
            if (c > 0)
            {
                ASSERT(c <= 9, "Invalid value of cell");
                if (seen[c])    return true;
                seen[c] = true;
            }
        }
    }

    //check contraditions along 3X3 block

    for (int outerY = 0; outerY <= 6; outerY += 3)
    {
        for (int outerX = 0; outerX <= 6; outerX += 3)
        {
            ResetArray(seen);
            for (int innerY = 0; innerY < 3; innerY++)
            {
                const int y = outerY + innerY;
                for (int innerX = 0; innerX < 3; innerX++)
                {
                    const int x = outerX + innerX;
                    unsigned char c = m_Cells[x + y * 9].GetValue();
                    if (c > 0)
                    {
                        ASSERT(c <= 9, "Invalid value of cell");
                        if (seen[c])    return true;
                        seen[c] = true;
                    }
                }
            }
        }
    }

    return false;
}

SolveState Board::SolveStep()
{
    if (IsSolved()) return SolveState::Solved;
    if (CheckContradiction())
    {
        //LOG_WARN("Contradiction");
        return SolveState::Contradiction;
    }

    //go through each column, Store a list of numbers that are known, and eliminate those numbers
    for (int x = 0; x < 9; x++)
    {
        short nSeen = 0;
        //calculate the seen numbers
        for (int y = 0; y < 9; y++)
        {
            unsigned char c = m_Cells[x + 9 * y].GetValue();
            if (c)
            {
                ASSERT(c < 10 && c >= 0, "Value of the cell is out of range.");
                nSeen |= (1 << (c-1));
            }
        }

        //Since we have calculated the seen numbers, remove it from the list of all possible numbers for that column
        for (int y = 0; y < 9; y++)
        {
            if (!m_Cells[x + y * 9].GetValue())
            {
                m_Cells[x + 9 * y].RemovePossibleL(nSeen);
            }
        }
    }

    //go through each row, Store a list of numbers that are fixed for that row (we know its value), and eliminate those 'seen' numbers from the possible values in that row
    for (int y = 0; y < 9; y++)
    {
        short nSeen = 0;
        //calculate the seen numbers
        for (int x = 0; x < 9; x++)
        {
            unsigned char c = m_Cells[x + 9 * y].GetValue();
            if (c)
            {
                ASSERT(c < 10 && c >= 0, "Value of the cell is out of range.");
                nSeen |= (1 << (c-1));
            }
        }

        //Since we have calculated the seen numbers, remove it from the list of all possible numbers for that row
        for (int x = 0; x < 9; x++)
        {
            if (!m_Cells[x + y * 9].GetValue())
            {
                m_Cells[x + 9 * y].RemovePossibleL(nSeen);
            }
        }
    }

    //go through each 3X3 block to remove possible values
    for (int outerY = 0; outerY <= 6; outerY += 3)
    {
        for (int outerX = 0; outerX <= 6; outerX += 3)
        {
            short nSeen = 0;
            //calculate the seen numbers in this 3x3 block
            for (int innerY = 0; innerY < 3; innerY++)
            {
                const int y = outerY + innerY;
                for (int innerX = 0; innerX < 3; innerX++)
                {
                    const int x = outerX + innerX;
                    unsigned char c = m_Cells[x + y * 9].GetValue();
                    if (c)
                    {
                        nSeen |= (1 << (c - 1));
                    }
                }
            }


            //Go through each element in this 3x3 block and remove the 'seen' numbers from the posssible values
            for (int innerY = 0; innerY < 3; innerY++)
            {
                const int y = outerY + innerY;
                for (int innerX = 0; innerX < 3; innerX++)
                {
                    const int x = outerX + innerX;

                    if (!m_Cells[x + y * 9].GetValue())
                    {
                        m_Cells[x + y * 9].RemovePossibleL(nSeen);
                    }
                    
                }
            }
        }
    }

    //Now check if a cell contains only one possibility.. If that is the case then you can set its value to the only possible value.
    for (int i = 0; i < 81; i++)
    {
        m_Cells[i].UpdateValue();
    }

    
    
    //Now check if a row has only one cell which can contain a number (Eg: if the number 9 is only possible at one place on the row, then u can set the value of that Cell to 9)
 
    Pair pairs[9];

    for (int y = 0; y < 9; y++)
    {
        memset(pairs, 0, sizeof(Pair) * 9);
        for (int x = 0; x < 9; x++)
        {
            Cell& cell = m_Cells[x + y * 9];
            if (cell.GetValue())    continue;   //The cell has a number in it

            //calculate the pairs 
            for (int i = 0; i < 9; i++)
            {
                if (cell.GetPossible(i+1))
                {
                    pairs[i].nCount++;
                    pairs[i].nx = x;
                    pairs[i].ny = y;
                }
            }
        }

        //Pairs have been calculated, now check if any of the occurences if 1
        //check if theres any digit which can only exist in one cell, if so then assign it
        for (unsigned char i = 0; i < 9; i++)
        {
            if (pairs[i].nCount == 1)
            {
                const int nIndex = pairs[i].nx + pairs[i].ny * 9;
                m_Cells[nIndex].SetValue(i + 1);
            }
        }
    }


    //Now check if a Column has only one cell which can contain a number (Eg: if the number 9 is only possible at one place on the column, then u can set the value of that Cell to 9)

    for (int x = 0; x < 9; x++)
    {
        memset(pairs, 0, sizeof(Pair) * 9);
        for (int y = 0; y < 9; y++)
        {
            Cell& cell = m_Cells[x + y * 9];
            if (cell.GetValue())    continue;   //The cell has a number in it

            //calculate the pairs 
            for (int i = 0; i < 9; i++)
            {
                if (cell.GetPossible(i + 1))
                {
                    pairs[i].nCount++;
                    pairs[i].nx = x;
                    pairs[i].ny = y;
                }
            }
        }

        //Pairs have been calculated, now check if any of the occurences if 1
        //check if theres any digit which can only exist in one cell, if so then assign it
        for (unsigned char i = 0; i < 9; i++)
        {
            if (pairs[i].nCount == 1)
            {
                const int nIndex = pairs[i].nx + pairs[i].ny * 9;
                m_Cells[nIndex].SetValue(i + 1);
            }
        }
    }

    //Now check if a 3X3 block has only one cell which can contain a number (Eg: if the number 9 is only possible at one place on the block, then u can set the value of that Cell to 9)

    for (int outerY = 0; outerY <= 6; outerY += 3)
    {
        for (int outerX = 0; outerX <= 6; outerX += 3)
        {
            memset(pairs, 0, sizeof(Pair) * 9);

            for (int innerY = 0; innerY < 3; innerY++)
            {
                const int y = outerY + innerY;
                for (int innerX = 0; innerX < 3; innerX++)
                {
                    const int x = outerX + innerX;

                    Cell& cell = m_Cells[x + y * 9];
                    if (cell.GetValue())    continue;   //The cell has a number in it

                    //calculate the pairs 
                    for (int i = 0; i < 9; i++)
                    {
                        if (cell.GetPossible(i + 1))
                        {
                            pairs[i].nCount++;
                            pairs[i].nx = x;
                            pairs[i].ny = y;
                        }
                    }

                }
            }

            //Pairs have been calculated, now check if any of the occurences if 1
        //check if theres any digit which can only exist in one cell, if so then assign it
            for (unsigned char i = 0; i < 9; i++)
            {
                if (pairs[i].nCount == 1)
                {
                    const int nIndex = pairs[i].nx + pairs[i].ny * 9;
                    m_Cells[nIndex].SetValue(i + 1);
                }
            }

        }
    }
    return SolveState::None;
}