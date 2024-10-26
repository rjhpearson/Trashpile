// Gamelogic.h

#include "TrashBlock.h"

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <windows.h>
#include "Sprite.h"

//void UpdateFrame(SPRITE* HERO, double deltaTime);
void UpdateFrame(SPRITE* HERO, double deltaTime, TRASH trashBlocks[]);
BOOL CheckCollision(SPRITE* hero, SPRITE* block);

//extern TRASH trashBlocks[MAX_TRASH_BLOCKS];

#endif // GAMELOGIC_H