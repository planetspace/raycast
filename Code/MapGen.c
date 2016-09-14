#include"MapGen.h"
#include"AILogic.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int LoadMap(Map* map, SpriteData* spriteData, const char* fileName)
{
	FILE* file;
	file = fopen(fileName, "r");
	//error check
	if(file == NULL)
	{
		printf("file: %s could not be opened!\n", fileName);
		return 1;
	}
	
	//load map size data
	fscanf(file, "%d", &map->width);
	fscanf(file, "%d", &map->height);
	
	//allocate memory for the width and check if it can be assigned
	map->data = malloc(map->width * sizeof(int*));
	if(map->data == NULL)
	{
		printf("malloc could not allocate: %lu bytes for map->data (width)\n", map->width * sizeof(int));
		return 1;
	}
	
	//allocate memory for the height and check if it can be assigned
	for(int i = 0; i < map->width; i++)
	{
		map->data[i] = malloc(map->height * sizeof(int));
		if(map->data[i] == NULL)
		{
			printf("malloc could not allocate: %lu bytes for map->data (height)\n", map->height * sizeof(int));
			free(map->data);
			return 1;
		}
	}
	
	//get spawn position
	fscanf(file, "%d", &map->spawnX);
	fscanf(file, "%d", &map->spawnY);
	
	//get map data
	for(int x = 0; x < map->width; x++)
	{
		for(int y = 0; y < map->height; y++)
		{
			fscanf(file, "%i", &map->data[x][y]);
		}
	}
	
	//get sprites / allocate the memory for them
	fscanf(file, "%d", &spriteData->numSprites);
	
	for(int i = 0; i < MAX_SPRITES; i++)
	{
		spriteData->order[i] = 0;
		spriteData->distance[i] = 0.0;
		InitSprite(&spriteData->sprite[i]);
	}

	for(int i = 0; i < spriteData->numSprites; i++)
	{
		//get the data for the sprites
		fscanf(file, "%lf", &spriteData->sprite[i].posX);
		fscanf(file, "%lf", &spriteData->sprite[i].posY);
		fscanf(file, "%lf", &spriteData->sprite[i].speed);
		fscanf(file, "%d", &spriteData->sprite[i].type);
		
		fscanf(file, "%lf", &spriteData->sprite[i].health);
		fscanf(file, "%lf", &spriteData->sprite[i].attack);
		
		fscanf(file, "%d", &spriteData->sprite[i].numFrames);
		fscanf(file, "%d", &spriteData->sprite[i].texture);
		spriteData->sprite[i].startFrame = spriteData->sprite[i].texture;
	}
	
	fclose(file);
	return 0;
}

void FreeMap(Map* map)
{
	for(int i = 0; i < map->height; i++)
	{
		free(map->data[i]);
	}
	free(map->data);
}

