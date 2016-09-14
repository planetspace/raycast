#include"AILogic.h"
#include<math.h>

void InitSprite(Sprite* sprite)
{
	sprite->posX = 0.0;
	sprite->posY = 0.0;
	sprite->dirX = 0.0;
	sprite->dirY = 0.0;
	sprite->speed = 0.0;
	sprite->type = 0;
	
	//combat
	sprite->health = 0.0;
	sprite->attack = 0.0;
	sprite->attackTime = 0.0;
	
	//animation
	sprite->startFrame = 0;
	sprite->numFrames = 0;
	sprite->animationTime = 0.0;
	sprite->texture = 0;
}

void RemoveSprite(SpriteData* spriteData, int index)
{
	if(index >= 0)
	{	
		spriteData->numSprites--;	
		
		Sprite temp = spriteData->sprite[spriteData->numSprites];
		spriteData->sprite[spriteData->numSprites] = spriteData->sprite[index];
		spriteData->sprite[index] = temp;		
	}
}

//TODO implement a*?
void UpdateSprite(Map* map, Player* player, SpriteData* spriteData, int index, double deltaTime)
{
	Sprite* sprite = &spriteData->sprite[index];
	if(sprite->health <= 0.0)
	{
		RemoveSprite(spriteData, index);
	}
	//calculate distance between player and sprite
	switch(sprite->type)
	{
		case PEACEFULL:
			//TODO need to implement merchants/npc
			break;
		case HOSTILE:
		{
			double distance = sqrt((player->posX - sprite->posX) * (player->posX - sprite->posX) + (player->posY - sprite->posY) * (player->posY - sprite->posY));
			sprite->dirX = -(sprite->posX - player->posX) / distance;
			sprite->dirY = -(sprite->posY - player->posY) / distance;

			double colSize = 4.5;
			if(distance < 9)
			{
				//check if move is possible then move to player
				double stepX = sprite->dirX * sprite->speed * deltaTime;
				double stepY = sprite->dirY * sprite->speed * deltaTime;
				if(!map->data[(int)(sprite->posX + (stepX * colSize))][(int)(sprite->posY + (stepY * colSize))])
				{
					sprite->posX += stepX;
					sprite->posY += stepY;
				}
				
				//attack
				sprite->attackTime += deltaTime;
				if(sprite->attackTime > sprite->attack)
				{
					sprite->attackTime -= sprite->attack;
					
					int index = spriteData->numSprites++;
					spriteData->sprite[index].posX = sprite->posX + sprite->dirX / 2;
					spriteData->sprite[index].posY = sprite->posY + sprite->dirY / 2;
					spriteData->sprite[index].dirX = sprite->dirX;
					spriteData->sprite[index].dirY = sprite->dirY;
					spriteData->sprite[index].speed = 4.4;
					spriteData->sprite[index].type = ENEMY_PROJECTILE;
				
					spriteData->sprite[index].health = 1.0;
					spriteData->sprite[index].attack = 0.3;
					spriteData->sprite[index].attackTime = 0.0;
				
					spriteData->sprite[index].startFrame = 4;
					spriteData->sprite[index].numFrames = 0;
					spriteData->sprite[index].animationTime = 0.0;
					spriteData->sprite[index].texture = 4;
				}
				
			}
		} break;
			
		case PLAYER_PROJECTILE:
		{
			sprite->posX += sprite->dirX * sprite->speed * deltaTime;
			sprite->posY += sprite->dirY * sprite->speed * deltaTime;
			
			if(map->data[(int)sprite->posX][(int)sprite->posY])
			{ //if we are colliding with a wall
				RemoveSprite(spriteData, index);
			}
			else
			{
				//check for sprite collision
				for(int i = 0; i < spriteData->numSprites; i++)
				{
					if(i == index)
					{
						//dont check for collisions against self
					}
					else if(spriteData->sprite[i].type == HOSTILE)
					{
						double distance = sqrt(	(spriteData->sprite[i].posX - sprite->posX) * (spriteData->sprite[i].posX - sprite->posX) + 
												(spriteData->sprite[i].posY - sprite->posY) * (spriteData->sprite[i].posY - sprite->posY));
						distance = fabs(distance);
						if(distance < 0.4)
						{
							//bullet
							RemoveSprite(spriteData, index);
							
							spriteData->sprite[i].health -= sprite->attack;
						}
					}
				}
				//check for player collisions
				double distance = sqrt((player->posX - sprite->posX) * (player->posX - sprite->posX) + (player->posY - sprite->posY) * (player->posY - sprite->posY));
				//if collision distance changes moddify player shot as well
				if(distance < 0.4)
				{
					printf("ouch!\n");
				}
			}
			if(sprite->posX <= 0 || sprite->posY <= 0 || sprite->posX >= map->width || sprite->posY >= map->height)
			{
				RemoveSprite(spriteData, index);
			}
			
		} break;
		
		case ENEMY_PROJECTILE:
		{
			sprite->posX += sprite->dirX * sprite->speed * deltaTime;
			sprite->posY += sprite->dirY * sprite->speed * deltaTime;
			
			if(map->data[(int)sprite->posX][(int)sprite->posY])
			{ //if we are colliding with a wall
				RemoveSprite(spriteData, index);
			}
			
		} break;
		
		case STATIC:
			break;
			
		default:
		 break;
	}
	
	//animate the sprite
	if(sprite->numFrames > 0)
	{
		sprite->animationTime += deltaTime;
		if(sprite->animationTime > 0.5)
		{
			sprite->animationTime -= 0.5;
			if(sprite->texture < sprite->startFrame + sprite->numFrames - 1)
				sprite->texture++;
			else
				sprite->texture = sprite->startFrame;
			
		}
	}
}
