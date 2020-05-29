#include "pch.h"
#include "Cell.h"
#include "Renderer/Renderer.h"
#include "Renderer/AssetManager.h"

Cell::Cell() :
   m_nPossibleValues(0x01FF),
   m_nValue(1)
{

}

static void GetQuadFromNumber(int digit, int gridX, int gridY, Quad& outQuad)
{
#if 0
   //Calculate color
   glm::vec4 colTopLeftBg = { 255,255,0,255 };
   glm::vec4 colTopRightBg = { 0,255,255,255 };
   glm::vec4 colBotLeftBg = { 255,0, 255,255 };
   glm::vec4 colBotRightBg = { 255,255,255,255 };
   colTopLeftBg *= 1.0f / 255.0f;
   colTopRightBg *= 1.0f / 255.0f;
   colBotLeftBg *= 1.0f / 255.0f;
   colBotRightBg *= 1.0f / 255.0f;

   //calculate colors
   //color of the left and right end of two horizontal lines (which have the same y coordinates as the points)
   glm::vec4 colHorizontaBL = MathR::LerpColor(colBotLeftBg, colTopLeftBg, gridY / 9.0);
   glm::vec4 colHorizontaBR = MathR::LerpColor(colBotRightBg, colTopRightBg, gridY / 9.0);
   glm::vec4 colHorizontaTL = MathR::LerpColor(colBotLeftBg, colTopLeftBg, (gridY+1) / 9.0);
   glm::vec4 colHorizontaTR = MathR::LerpColor(colBotRightBg, colTopRightBg, (gridY+1) / 9.0);

   //Color of the vertices
   glm::vec4 colBL = MathR::LerpColor(colHorizontaBL, colHorizontaBR, (gridX) / 9.0);
   glm::vec4 colBR = MathR::LerpColor(colHorizontaBL, colHorizontaBR, (gridX+1) / 9.0);
   glm::vec4 colTL = MathR::LerpColor(colHorizontaTL, colHorizontaTR, (gridX) / 9.0);
   glm::vec4 colTR = MathR::LerpColor(colHorizontaTL, colHorizontaTR, (gridX+1) / 9.0);

#else
   glm::vec4 colBL = glm::vec4 {0.8,0.4, 0.1, 1.0};
   glm::vec4 colBR = glm::vec4 {0.8,0.4, 0.1, 1.0};
   glm::vec4 colTL = glm::vec4 {0.8,0.4, 0.1, 1.0};
   glm::vec4 colTR = glm::vec4 {0.8,0.4, 0.1, 1.0};
#endif
   //Useful constants
   const float dx = (g_rectBoard.z - g_rectBoard.x)/9.0f;
   const float dy = (g_rectBoard.w - g_rectBoard.y)/9.0f;
   const float startingX = g_rectBoard.x + gridX * dx;
   const float startingY = g_rectBoard.y + gridY * dy;

   const float texdx = 1.0f/5.0f;
   const float texdy = 1.0f/2.0f;
   
   int texCordX = ((digit - 1) % 5);
   int texCordY = 1 - ((digit - 1) / 5);
   float texX = texCordX/5.0f;
   float texY = texCordY /2.0f;
   
   outQuad.m_vertices[0] = Vertex{ {startingX, startingY},        colBL, 1, {texX, texY} };
   outQuad.m_vertices[1] = Vertex{ {startingX+dx, startingY},     colBR, 1, {texX + texdx, texY} };
   outQuad.m_vertices[2] = Vertex{ {startingX+dx, startingY+dy},  colTR, 1, {texX + texdx, texY+texdy} };
   outQuad.m_vertices[3] = Vertex{ {startingX, startingY+dy},     colTL, 1, {texX, texY + texdy} };
}

void Cell::OnRender(int x, int y)
{
   if (m_nValue > 0 && m_nValue < 10)
   {
      Quad quad;
      GetQuadFromNumber(m_nValue, x, y, quad);
      Renderer::DrawQuad(quad);
   }
}