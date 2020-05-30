#pragma once
#include "Log/Log.h"

extern glm::vec4 g_rectBoard;

class Cell
{
public:
   Cell();
   inline bool GetPossible(int digit) const { 
      ASSERT(digit >= 1 && digit <= 9, "Invalid digit"); 
      return m_nPossibleValues & (1 << (digit - 1));
   }

   inline void SetPossible (int digit, bool state) { 
      ASSERT(digit >= 1 && digit <= 9, "Invalid digit"); 
      if (state) m_nPossibleValues |= 1 << (digit - 1);
      else       m_nPossibleValues &= ~(1 << (digit - 1));
   }

   inline unsigned char GetValue() const { return m_nValue; }
   inline void SetValue(unsigned char c) { m_nValue = c; }

   void OnRender(int x, int y);
   void OnRenderSelected(int x, int y);

   void Clear();
private:
   //bitmask... LSB stores digit 1. Value at that bit is true if its value is possible
   unsigned short m_nPossibleValues;
   unsigned char m_nValue;
};