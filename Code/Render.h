#ifndef RENDER_H
#define RENDER_H

#include"DataStructs.h"
#include"Time.h"

//draw textured walls
void RenderWalls(Window* win, Map* map, Player* player, SpriteData* spriteData, TextureData* tex);
void RenderUI(Window* win, Map* map);

//render help functions
void SortSprites(int* order, double* dist, int amount);

//displays the buffer to the screen
void DisplayBuffer(Window* win, FrameTime* time);

//helperfunction to draw primitives to the buffer
void DrawRect(Window* win, Uint32 x, Uint32 y, Uint32 width, Uint32 height, Uint32 color);
void DrawLine(Window* win, Uint32 x, Uint32 y1, Uint32 y2, Uint32 color);
void DrawPixel(Window* win, Uint32 x, Uint32 y, Uint32 color);

#endif //RENDER_H
