#pragma once
#include "Cell.h"

class Board
{
public:
   Board();

   void OnRender();

   inline Board* GetNext() const { return m_nextBoard; }
private:
   Cell m_Cells[81];
   Board* m_nextBoard;  //link list
};