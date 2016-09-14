#include"DataStructs.h"
#include"InitSDL.h"
#include"Input.h"
#include"AILogic.h"
#include"Time.h"
#include"Render.h"
#include"MapGen.h"

int main(/*int argc, char* args[]*/)
{
	Window window = {};
	TextureData textureData = {};
	FrameTime time = {};
	
	if(!InitSDL(&window, &time) || !LoadBMP(&textureData))
		return 1;
	
	Map map = {};
	SpriteData spriteData = {};
	if(LoadMap(&map, &spriteData, "Resources/Maps/Map1.txt"))
		return 1;	
	
	//initialize random number gen
	
	
	Player player = {};
	//player
	player.posX = map.spawnX;
	player.posY = map.spawnY;
	
	player.action = NO_ACTION;
	//camera
	player.dirX = -1.0;
	player.dirY = 0.0;
	player.planeX = 0.0;
	player.planeY = 0.66;
	//speed
	player.speed = 3.5;
	player.rotateSpeedX = 0.08;
	
	double deltaTime;

	while(HandleInput(&player))
	{
		//rendering
		RenderWalls(&window, &map, &player, &spriteData, &textureData);
		RenderUI(&window, &map);
		DisplayBuffer(&window, &time);
		
		//update
		deltaTime = CalcFPS(&time);
		UpdatePlayer(&map, &player, &spriteData, deltaTime);
		for(int i = 0; i < spriteData.numSprites; i++)
			UpdateSprite(&map, &player, &spriteData, i, deltaTime);
	}
	
	FreeMap(&map);
	CloseSDL(&window, &textureData);
	
	return 0;
}

