#ifndef AILOGIC_H
#define AILOGIC_H

#include"DataStructs.h"

//sprite functions
void InitSprite(Sprite* sprite);
void RemoveSprite(SpriteData* spriteData, int index);
void UpdateSprite(Map* map, Player* player, SpriteData* spriteData, int index, double deltaTime);

#endif //AILOGIC_H
