#pragma once
#include "Cell.h"

enum class SolveState : unsigned int 
{
	None = 0,
	Contradiction,
	Solved,
	CreateGuess
};
class Board
{
public:
   Board();

   void OnRender();

   inline Board* GetNext() const { return m_nextBoard; }
   inline void SetNext(Board* pNext) { m_nextBoard = pNext; }
   void ClearBoard();
   void ClearSelection() { m_pCurrentCell = nullptr;  }
   void ResetPossibleValues();
   
   //Events
   void OnMouseDown(double posX, double posY);
   void OnKey(int key);
   
   void LoadBoard(unsigned char data[81]);
   SolveState SolveStep();

private:
	//the nCount stores the number of possible occurences for a particular number. The x,y coordinates store the Cell location. If the count is 1, then Cell[x][y] must contain that digit (and that digit is given by the index position in the array)
	struct Pair
	{
		unsigned char nCount;
		unsigned char nx;
		unsigned char ny;
	};

private:
	bool IsSolved() const;
	bool CheckContradiction() const;
	//the function calculates the pairs for any given cell. The pairs stores the number of places where a particular number is possible within a row/column/block. So in an entire row if the number 9 can exist in only one place, then that cell must contain a 9. (consequently the nCount would be one.) The index into the array stores the digit value (Its like a map with the digit as the key). So the pair with value nCount = 2 would be stored at index 8 (because the value of the number was 9 in the example)
	//void CalculateUniqueCells(int x, int y, Pair pairs[]);
private:
   Cell m_Cells[81];
   Cell* m_pCurrentCell;
   int m_nSelectedX;
   int m_nSelectedY;
   Board* m_nextBoard;  //link list
};