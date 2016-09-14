#ifndef INPUT_H
#define INPUT_H

#include"DataStructs.h"

int HandleInput(Player* player);
void UpdatePlayer(Map* map, Player* player, SpriteData* spriteData, double deltaTime);

#endif //INPUT_H
