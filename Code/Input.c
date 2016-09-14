#include"Input.h"
#include<SDL2/SDL.h>

#define FORWARD 		(1 << 0)
#define BACK			(1 << 1)
#define LEFT			(1 << 2)
#define RIGHT			(1 << 3)
#define STRAFE_LEFT		(1 << 4)
#define STRAFE_RIGHT	(1 << 5)

int HandleInput(Player* player)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				return 0;
				break;
				
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						return 0;
						break;
					
					case SDLK_UP:
					case 'w':
						player->movement |= FORWARD;
						break;
						
					case SDLK_DOWN:
					case 's':
						player->movement |= BACK;
						break;
						
					case SDLK_LEFT:
					case 'a':
						player->movement |= LEFT;
						break;
					
					case SDLK_RIGHT:
					case 'd':
						player->movement |= RIGHT;
						break;
						
					case 'q':
						player->movement |= STRAFE_LEFT;
						break;
						
					case 'e':
						player->movement |= STRAFE_RIGHT;
						break;
					
					case SDLK_SPACE:
						player->action = OPEN_DOOR;
						break;
					
					case 'f':
						player->action = FIRE_SPELL;
						break;
					
					default:
						break;
				}
				break;
				
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
					case 'w':
						player->movement &= (~FORWARD);
						break;
						
					case SDLK_DOWN:
					case 's':
						player->movement &= (~BACK);
						break;
						
					case SDLK_LEFT:
					case 'a':
						player->movement &= (~LEFT);
						break;
					
					case SDLK_RIGHT:
					case 'd':
						player->movement &= (~RIGHT);
						break;
					
					case 'q':
						player->movement &= (~STRAFE_LEFT);
						break;
						
					case 'e':
						player->movement &= (~STRAFE_RIGHT);
						break;
						
					case SDLK_SPACE:
						player->action = NO_ACTION;
						break;
					
					case 'f':
						player->action = NO_ACTION;
						break;
					
					default:
						break;
				}
				break;
			
			case SDL_MOUSEMOTION:
				break;
				
			default:
				break;
		}
	}
	
	return 1;
}

void UpdatePlayer(Map* map, Player* player, SpriteData* spriteData, double deltaTime)
{
	//TODO: movement speed is increased when pressing strafe + forward/back	?normalize vectors?
	float colSize = 4.5f;
	
	if(player->movement & FORWARD)
	{
		double stepX = player->dirX * deltaTime * player->speed;
		double stepY = player->dirY * deltaTime * player->speed;
		
		if(!map->data[(int)(player->posX + (stepX * colSize))][(int)player->posY])
			player->posX += stepX;
		if(!map->data[(int)player->posX][(int)(player->posY + (stepY * colSize))])
			player->posY += stepY;
	}
	if(player->movement & BACK)
	{
		double stepX = -player->dirX * deltaTime * player->speed;
		double stepY = -player->dirY * deltaTime * player->speed;
		
		if(!map->data[(int)(player->posX + (stepX * colSize))][(int)player->posY])
			player->posX += stepX;
		if(!map->data[(int)player->posX][(int)(player->posY + (stepY * colSize))])
			player->posY += stepY;
	}
	if(player->movement & LEFT)
	{
		double oldDirX = player->dirX;
		player->dirX = player->dirX * cos(player->rotateSpeedX) - player->dirY * sin(player->rotateSpeedX);
		player->dirY = oldDirX * sin(player->rotateSpeedX) + player->dirY * cos(player->rotateSpeedX);
		double oldPlaneX = player->planeX;
		player->planeX = player->planeX * cos(player->rotateSpeedX) - player->planeY * sin(player->rotateSpeedX);
		player->planeY = oldPlaneX * sin(player->rotateSpeedX) + player->planeY * cos(player->rotateSpeedX);
	}
	if(player->movement & RIGHT)
	{
		double oldDirX = player->dirX;
		player->dirX = player->dirX * cos(-player->rotateSpeedX) - player->dirY * sin(-player->rotateSpeedX);
		player->dirY = oldDirX * sin(-player->rotateSpeedX) + player->dirY * cos(-player->rotateSpeedX);
		double oldPlaneX = player->planeX;
		player->planeX = player->planeX * cos(-player->rotateSpeedX) - player->planeY * sin(-player->rotateSpeedX);
		player->planeY = oldPlaneX * sin(-player->rotateSpeedX) + player->planeY * cos(-player->rotateSpeedX);
	}
	if(player->movement & STRAFE_LEFT)
	{
		double stepX = -player->dirY * deltaTime * (player->speed / 1.5);
		double stepY = player->dirX * deltaTime * (player->speed / 1.5);
		
		if(!map->data[(int)(player->posX + (stepX * colSize))][(int)player->posY])
			player->posX += stepX;
		if(!map->data[(int)player->posX][(int)(player->posY + (stepY * colSize))])
			player->posY += stepY;
	}
	if(player->movement & STRAFE_RIGHT)
	{
		double stepX = player->dirY * deltaTime * (player->speed / 1.5);
		double stepY = -player->dirX * deltaTime * (player->speed / 1.5);
		
		if(!map->data[(int)(player->posX + (stepX * colSize))][(int)player->posY])
			player->posX += stepX;
		if(!map->data[(int)player->posX][(int)(player->posY + (stepY * colSize))])
			player->posY += stepY;
	}
	
	switch(player->action)
	{
		case OPEN_DOOR:
			if(map->data[(int)(player->posX + player->dirX)][(int)(player->posY + player->dirY)] == 2)
			map->data[(int)(player->posX + player->dirX)][(int)(player->posY + player->dirY)] = 0;
			break;
		
		case FIRE_SPELL:
		{
			if(spriteData->numSprites >= MAX_SPRITES)
			{
				printf("No more room for sprites!");
				break;			
			}
			else
			{
				int index = spriteData->numSprites++;
				spriteData->sprite[index].posX = player->posX + player->dirX / 2;
				spriteData->sprite[index].posY = player->posY + player->dirY / 2;
				spriteData->sprite[index].dirX = player->dirX;
				spriteData->sprite[index].dirY = player->dirY;
				spriteData->sprite[index].speed = 4.4;
				spriteData->sprite[index].type = PLAYER_PROJECTILE;
				
				spriteData->sprite[index].health = 1.0;
				spriteData->sprite[index].attack = 0.3;
				spriteData->sprite[index].attackTime = 0.0;
				
				spriteData->sprite[index].startFrame = 4;
				spriteData->sprite[index].numFrames = 0;
				spriteData->sprite[index].animationTime = 0.0;
				spriteData->sprite[index].texture = 4;

				player->action = NO_ACTION;
				printf("NUM_SPRITES: %d\n", spriteData->numSprites);
			}
		}break;
			
		case NO_ACTION:
			break;
		default:
			break;
	}
}
