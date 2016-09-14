#ifndef MAPGEN_H
#define MAPGEN_H

#include"DataStructs.h"

int LoadMap(Map* map, SpriteData* spriteData, const char* fileName);
void FreeMap(Map* map);

#endif //MAPGEN_H
