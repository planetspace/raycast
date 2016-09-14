#ifndef INITSDL_H
#define INITSDL_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_opengl.h>
#include"DataStructs.h"

int InitSDL(Window* win, FrameTime* time);
int LoadBMP(TextureData* tex);

Uint32 GetPixel(SDL_Surface* surface, int x, int y);

void CloseSDL(Window* win, TextureData* tex);

#endif //INITSDL_H
