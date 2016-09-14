#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include<SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 800

#define RENDER_WIDTH 1040
#define RENDER_HEIGHT 700

#define MAX_SPRITES 300

#define TEXTURE_SIZE 64
#define NUM_TEXTURES 8

typedef struct render_window
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	Uint32 buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
} Window;

typedef struct texture_data
{
	SDL_Surface* source[NUM_TEXTURES];
	Uint32 pixelData[NUM_TEXTURES][TEXTURE_SIZE][TEXTURE_SIZE];
} TextureData;

typedef struct frame_time_data
{
	double currentTime;
	double prevTime;
	int accumulatedFPS;
	int numFrames;
	char currentFPS[5];
	TTF_Font* font;
} FrameTime;

typedef struct map_data
{
	int width;
	int height;
	int spawnX;
	int spawnY;
	int **data;
} Map;

typedef enum player_action
{
	OPEN_DOOR,
	FIRE_SPELL,
	NO_ACTION
} Action;	

typedef struct player_data
{
	double posX;
	double posY;
	
	double dirX;
	double dirY;
	
	char movement;
	Action action;
	
	double planeX;
	double planeY;
	
	double speed;
	double rotateSpeedX;
	
	int spellIndex;
} Player;

typedef enum sprite_behavior
{
	PEACEFULL = 0,
	HOSTILE = 1,
	PLAYER_PROJECTILE = 2,
	ENEMY_PROJECTILE = 3,
	STATIC = 4
} behavior;

typedef struct Entity_Sprite
{
	double posX;
	double posY;
	
	double dirX;
	double dirY;
	
	double speed;
	
	int type;
	
	//combat
	double health;
	double attack;
	double attackTime;
	
	//animation
	int startFrame;
	int numFrames;
	double animationTime;
	int texture;
} Sprite;

typedef struct Sprite_Data
{
	int numSprites;
	double zBuffer[RENDER_WIDTH];
	int order[MAX_SPRITES];
	double distance[MAX_SPRITES];
	Sprite sprite[MAX_SPRITES];
} SpriteData;

#endif //DATASTRUCTS_H
