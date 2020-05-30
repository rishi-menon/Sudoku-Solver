#pragma once
#include "Cell.h"

class Board
{
public:
   Board();

   void OnRender();

   inline Board* GetNext() const { return m_nextBoard; }

   void OnMouseDown(double posX, double posY);
   void OnKeyDown(int key);

   inline void SetNext(Board* pNext) { m_nextBoard = pNext; }
   void ClearBoard();

private:
   Cell m_Cells[81];
   Cell* m_pCurrentCell;
   int m_nSelectedX;
   int m_nSelectedY;
   Board* m_nextBoard;  //link list
};