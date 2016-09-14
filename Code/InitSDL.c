#include"InitSDL.h"

int InitSDL(Window* win, FrameTime* time)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	win->window = SDL_CreateWindow("hello world", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_FULLSCREEN_DESKTOP);
	win->renderer = SDL_CreateRenderer(win->window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(win->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	if(win->window == NULL)
	{
		printf("SDL window could not be created! SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	if(win->renderer == NULL)
	{
		printf("SDL failed to set the screen! SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	win->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if(win->texture == NULL)
	{
		printf("SDL texture failed to initialize! SDL error: %s\n", SDL_GetError());
		return 0;
	}
	
	if(TTF_Init() != 0)
	{
		printf("TTF could not load: %s\n", SDL_GetError());
		return 0;
	}
	
	time->font = TTF_OpenFont("Resources/Font/FreeSans.ttf", 16);
	if(time->font == NULL)
	{
		printf("Failed to load font: %s\n", SDL_GetError());
		return 0;
	}
	
	return 1;
}

int LoadBMP(TextureData* tex)
{
	for(int i = 0; i < NUM_TEXTURES; i++)
	{
		//NOTE: malloc clears just enough room for the current filepath
		char* imagePath = malloc(30);
		sprintf(imagePath, "Resources/Images/texture%d.bmp", i);
		
		tex->source[i] = SDL_LoadBMP(imagePath);
		if(tex->source[i] == NULL)
		{
			printf("unable to load Resources/Images/texture%d.bmp, SDL error: %s\n", i, SDL_GetError());
			free(imagePath);
			return 0;
		}
		
		free(imagePath);
	}
	
	for(int t = 0; t < NUM_TEXTURES; t++)
	{
		for(int x = 0; x < TEXTURE_SIZE; x++)
		{
			for(int y = 0; y < TEXTURE_SIZE; y++)
			{
				tex->pixelData[t][x][y] = GetPixel(tex->source[t], x, y);
			}
		}
	}
	
	return 1;
}

Uint32 GetPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	switch(bpp)
	{
		case 1:
			return *p;
			break;
		case 2:
			return *(Uint16*)p;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;
		case 4:	
			return *(Uint32*)p;
			break;
		default:
			return 0;
			break;
	}
}

void CloseSDL(Window* win, TextureData* tex)
{
	for(int i = 0; i < NUM_TEXTURES; i++)
	{
		SDL_FreeSurface(tex->source[i]);
		tex->source[i] = NULL;
	}
	
	SDL_DestroyWindow(win->window);
	win->window = NULL;
	
	TTF_Quit();
	SDL_Quit();
}
//https://github.com/wenright/Raycast-Engine/blob/master/src/draw.c
