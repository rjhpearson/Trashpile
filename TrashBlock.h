// TrashBlock.h

#ifndef TRASHBLOCK_H
#define TRASHBLOCK_H

#include <windows.h>
#include "Sprite.h"
#include "Config.h"

typedef struct TRASH {
    SPRITE sprite;
    BOOL active;
    BOOL display;
} TRASH;

extern TRASH trashBlocks[MAX_TRASH_BLOCKS];
extern HDC hdcBackBuffer;

void InitializeTrashBlocks();
void UpdateTrashBlocks(double deltaTime);
void RenderTrashBlocks(HDC hdcBackBuffer);

#endif // TRASHBLOCK_H
