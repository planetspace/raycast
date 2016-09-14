#include"Render.h"
#include<math.h>

void RenderWalls(Window* win, Map* map, Player* player, SpriteData* spriteData, TextureData* tex)
{
	for(int x = 0; x < RENDER_WIDTH; x++)
	{
		double cameraX = 2 * x / (double)RENDER_WIDTH - 1;
		double rayPosX = player->posX;
		double rayPosY = player->posY;
		double rayDirX = player->dirX + player->planeX * cameraX;
		double rayDirY = player->dirY + player->planeY * cameraX;
		
		int mapX = (int)rayPosX;
		int mapY = (int)rayPosY;
		
		double sideDistX;
		double sideDistY;
		
		double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
		double perpDistWall;
		
		int stepX;
		int stepY;
		int hit = 0;
		int side;
		
		if(rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (rayPosX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
		}
		if(rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (rayPosY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
		}
		
		while(hit == 0)
		{
			if(sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			
			if(map->data[mapX][mapY] > 0)
				hit = 1;			
		}
		
		if(side == 0)
			perpDistWall = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
		else
			perpDistWall = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;
		
		int lineHeight = (int)(RENDER_HEIGHT / perpDistWall * 1.3);
		
		int drawStart = -lineHeight / 2 + RENDER_HEIGHT / 2;
		if(drawStart < 0)
			drawStart = 0;
			
		int drawEnd = lineHeight / 2 + RENDER_HEIGHT / 2;
		if(drawEnd >= RENDER_HEIGHT)
			drawEnd = RENDER_HEIGHT;
		
		int texNum = map->data[mapX][mapY] - 1;
		
		double wallX;
		if(side == 0)
			wallX = rayPosY + perpDistWall * rayDirY;
		else
			wallX = rayPosX + perpDistWall * rayDirX;
		wallX -= floor(wallX);
		
		int texX = (int)(wallX * (double)TEXTURE_SIZE);
		if(side == 0 && rayDirX > 0)
			texX = TEXTURE_SIZE - texX -1;
		if(side == 1 && rayDirY < 0)
			texX = TEXTURE_SIZE - texX -1;
		
		for(int y = drawStart; y < drawEnd; y++)
		{
			int d = y * 256 - RENDER_HEIGHT * 128 + lineHeight * 128;
			int texY = ((d * TEXTURE_SIZE) / lineHeight) / 256;
			Uint32 color = tex->pixelData[texNum][texX][texY];
			if(side == 1)
				color = (color >> 1) & 8355711;
			
			DrawPixel(win, x, y, color);
		}	
		
		//draw floor
		double floorXWall;
		double floorYWall;
		
		if(side == 0 && rayDirX > 0)
		{
			floorXWall = mapX;
			floorYWall = mapY + wallX;
		}
		else if(side == 0 && rayDirX < 0)
		{
			floorXWall = mapX + 1.0;
			floorYWall = mapY + wallX;
		}
		else if(side == 1 && rayDirY > 0)
		{
			floorXWall = mapX + wallX;
			floorYWall = mapY;
		}
		else
		{
			floorXWall = mapX + wallX;
			floorYWall = mapY + 1.0;
		}		
		double distWall = perpDistWall;
		double distPlayer = 0.0;
		double currentDist;
		
		if(drawEnd < 0)
			drawEnd = RENDER_HEIGHT;
		
		for(int y = drawEnd; y < RENDER_HEIGHT; y++)
		{
			currentDist = RENDER_HEIGHT / (2.0 * y - RENDER_HEIGHT);
			double weight = (currentDist - distPlayer) / (distWall - distPlayer);
			
			double currentFloorX = weight * floorXWall + (1.0 - weight) * player->posX;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * player->posY;
			
			int floorTexX = (int)(currentFloorX * TEXTURE_SIZE) % TEXTURE_SIZE;
			int floorTexY = (int)(currentFloorY * TEXTURE_SIZE) % TEXTURE_SIZE;
			
			DrawPixel(win, x, (RENDER_HEIGHT - (y + 1)), tex->pixelData[3][floorTexX][floorTexY]);
			DrawPixel(win, x, y, (tex->pixelData[3][floorTexX][floorTexY] >> 1) & 8355711);
		}
		
		//render sprites
				
		spriteData->zBuffer[x] = perpDistWall;
		
	}
	
	for(int i = 0; i < spriteData->numSprites; i++)
	{
		spriteData->order[i] = i;
		spriteData->distance[i] = ((player->posX - spriteData->sprite[i].posX) * (player->posX - spriteData->sprite[i].posX) + 
									(player->posY - spriteData->sprite[i].posY) * (player->posY - spriteData->sprite[i].posY));
	}
	SortSprites(spriteData->order, spriteData->distance, spriteData->numSprites);
	
	for(int i = 0; i < spriteData->numSprites; i++)
	{
		double spriteX = spriteData->sprite[spriteData->order[i]].posX - player->posX;
		double spriteY = spriteData->sprite[spriteData->order[i]].posY - player->posY;
		
		double invDet = 1.0 / (player->planeX * player->dirY - player->dirX * player->planeY);
		double transformX = invDet * (player->dirY * spriteX - player->dirX * spriteY);
		double transformY = invDet * (-player->planeY * spriteX + player->planeX * spriteY);
		
		int spriteScreenX = (int)((RENDER_WIDTH / 2) * (1 + transformX / transformY));
		
		int spriteHeight = abs((int)(RENDER_HEIGHT / (transformY)));
		int drawStartY = -spriteHeight / 2 + RENDER_HEIGHT / 2;
		if(drawStartY < 0)
			drawStartY = 0;
		
		int drawEndY = spriteHeight / 2 + RENDER_HEIGHT / 2;
		if(drawEndY >= RENDER_HEIGHT)
			drawEndY = RENDER_HEIGHT -1;
		
		int spriteWidth = abs((int)(RENDER_HEIGHT / (transformY)));
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if(drawStartX < 0)
			drawStartX = 0;
		
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if(drawEndX >= RENDER_WIDTH)
			drawEndX = RENDER_WIDTH -1;
		
		for(int x = drawStartX; x < drawEndX; x++)
		{
			int texX = (int)(256 * (x - (-spriteWidth / 2 + spriteScreenX)) * TEXTURE_SIZE / spriteWidth) / 256;
			if(transformY > 0 && x > 0 && x < RENDER_WIDTH && transformY < spriteData->zBuffer[x])
			{
				for(int y = drawStartY; y < drawEndY; y++)
				{
					int d = (y) * 256 - RENDER_HEIGHT * 128 + spriteHeight * 128;
					int texY = ((d * TEXTURE_SIZE) / spriteHeight) / 256;
					Uint32 color = tex->pixelData[spriteData->sprite[spriteData->order[i]].texture][texX][texY];
					if((color & 0x00FFFFFF) != 0)
					win->buffer[y][x] = color;
				}
			}
		}
		
	}
}

void RenderUI(Window* win, Map* map)
{
	//health bar
	DrawRect(win, 0, RENDER_HEIGHT, RENDER_WIDTH, SCREEN_HEIGHT - RENDER_HEIGHT, 20000);
	//inventory
	DrawRect(win, RENDER_WIDTH, 0, SCREEN_WIDTH - RENDER_WIDTH, SCREEN_HEIGHT, 800099);
}

void SortSprites(int* order, double* dist, int amount)
{
	int gap = amount;
	char swapped = 0;
	while(gap > 1 || swapped)
	{
		gap = (gap * 10) / 13;
		if(gap == 9 || gap == 10)
			gap = 11;
		if(gap < 0)
			gap = 1;
		swapped = 0;
		for(int i = 0; i < amount - gap; i++)
		{
			int j = i + gap;
			if(dist[i] < dist[j])
			{
				//swap dist
				int temp = dist[i];
				dist[i] = dist[j];
				dist[j] = temp;
				//swap order
				double tmp = order[i];
				order[i] = order[j];
				order[j] = tmp;
							
				swapped = 1;
			}
		}
	}
}

void DisplayBuffer(Window* win, FrameTime* time)
{
	SDL_UpdateTexture(win->texture, NULL, win->buffer, SCREEN_WIDTH * 4);
	SDL_RenderClear(win->renderer);
	SDL_RenderCopy(win->renderer, win->texture, NULL, NULL);
	
	RenderFPS(win, time);
	
	SDL_RenderPresent(win->renderer);
}

void DrawRect(Window* win, Uint32 x, Uint32 y, Uint32 width, Uint32 height, Uint32 color)
{
	for(Uint32 i = x; i < x + width; i++)
	{
		for(Uint32 j = y; j < y + height; j++)
		{
			win->buffer[j][i] = color;
		}
	}
}

void DrawLine(Window* win, Uint32 x, Uint32 y1, Uint32 y2, Uint32 color)
{
	for(Uint32 i = y1; i < y2; i++)
	{
		win->buffer[i][x] = color;
	}
}

void DrawPixel(Window* win, Uint32 x, Uint32 y, Uint32 color)
{
	if(x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
		return;
	
	win->buffer[y][x] = color;
}
