#include"Time.h"
#include"DataStructs.h"
#include<SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

double CalcFPS(FrameTime* time)
{
	time->prevTime = time->currentTime;
	time->currentTime = clock();
	double deltaTime = (time->currentTime - time->prevTime) / CLOCKS_PER_SEC;
	int intFPS = 1 / deltaTime;
	time->accumulatedFPS += intFPS;
	time->numFrames++;
	
	if(time->numFrames > 120)
	{
		sprintf(time->currentFPS, "%d", time->accumulatedFPS / time->numFrames);
		time->numFrames = 0;
		time->accumulatedFPS = 0;
	}
	
	return deltaTime;
}

void RenderFPS(Window* win, FrameTime* time)
{
	SDL_Color white = {255, 255, 255, 255};
	
	SDL_Surface* fpsTexSurface = TTF_RenderText_Solid(time->font, time->currentFPS, white);
	SDL_Texture* fpsTexture = SDL_CreateTextureFromSurface(win->renderer, fpsTexSurface);
	
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 50;
	rect.h = 50;
	
	SDL_RenderCopy(win->renderer, fpsTexture, NULL, &rect);
}
