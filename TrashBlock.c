// TrashBlock.c

#include "TrashBlock.h"
#include <windows.h>
#include <stdlib.h>
#include <time.h>

TRASH trashBlocks[MAX_TRASH_BLOCKS];
int currentBlock = 0;

void InitializeTrashBlocks() {
    //srand((unsigned int)time(NULL)); // Seed once
    for (int i = 0; i < MAX_TRASH_BLOCKS; i++) {
        trashBlocks[i].sprite.x = generateCenteredMultipleOf16();
        trashBlocks[i].sprite.y = 0;
        trashBlocks[i].sprite.frameCount = FRAME_COUNT;
        trashBlocks[i].sprite.currentFrame = 0;
        trashBlocks[i].sprite.velocityY = 1; // Small initial velocity
        trashBlocks[i].active = FALSE;
        trashBlocks[i].display = FALSE;
        LoadBitmapsToMemory(hdcBackBuffer, TRASHFilePaths, trashBlocks[i].sprite.hbitmaps, trashBlocks[i].sprite.bitmaps, trashBlocks[i].sprite.frameCount);
    }
    // Start the first block
    trashBlocks[0].active = TRUE;
    trashBlocks[0].display = TRUE;
}

void UpdateTrashBlocks(double deltaTime) {
    for (int i = 0; i < MAX_TRASH_BLOCKS; i++) {
        if (trashBlocks[i].active) {
            // Apply gravity proportional to deltaTime
            trashBlocks[i].sprite.velocityY += 0.1; // GRAVITY; // *deltaTime;
            trashBlocks[i].sprite.y += trashBlocks[i].sprite.velocityY; // *deltaTime;

            // Collision with ground
            if (trashBlocks[i].sprite.y >= 224) {
                trashBlocks[i].sprite.y = 224;
                trashBlocks[i].sprite.velocityY = 0;
                trashBlocks[i].active = FALSE;

                if (i + 1 < MAX_TRASH_BLOCKS) {
                    trashBlocks[i + 1].sprite.y = 0; // Reset to start falling
                    trashBlocks[i + 1].sprite.x = generateCenteredMultipleOf16();
                    trashBlocks[i + 1].active = TRUE;
                    trashBlocks[i + 1].display = TRUE;
                }
            }

            // Collision with other blocks
            for (int j = 0; j < i; j++) { // Only check previous blocks
                if (trashBlocks[i].sprite.x == trashBlocks[j].sprite.x &&
                    trashBlocks[i].sprite.y + SPRITE_SIZE >= trashBlocks[j].sprite.y) {
                    trashBlocks[i].sprite.y = trashBlocks[j].sprite.y - SPRITE_SIZE;
                    trashBlocks[i].sprite.velocityY = 0;
                    trashBlocks[i].active = FALSE;

                    if (i + 1 < MAX_TRASH_BLOCKS) {
                        trashBlocks[i + 1].sprite.y = 0; // Reset to start falling
                        trashBlocks[i + 1].sprite.x = generateCenteredMultipleOf16();
                        trashBlocks[i + 1].active = TRUE;
                        trashBlocks[i + 1].display = TRUE;
                    }
                    break;
                }
            }
        }
    }
}


void RenderTrashBlocks(HDC hdcBackBuffer) {
    HDC hdcMem = CreateCompatibleDC(hdcBackBuffer);
    for (int i = 0; i < MAX_TRASH_BLOCKS; i++) {
        if (trashBlocks[i].display) {
            SelectObject(hdcMem, trashBlocks[i].sprite.hbitmaps[trashBlocks[i].sprite.currentFrame]);
            BitBlt(hdcBackBuffer, trashBlocks[i].sprite.x, trashBlocks[i].sprite.y, SPRITE_SIZE, SPRITE_SIZE, hdcMem, 0, 0, SRCCOPY);
        }
    }
    DeleteDC(hdcMem);
}

int generateCenteredMultipleOf16() {
    int min = 64;
    int max = 256;
    int mid = (min + max) / 2;
    int range = (max - min) / 16 + 1;
    int randomOffset = rand() % (range / 2 + 1);  // Half the range to favor center
    int sign = rand() % 2 == 0 ? -1 : 1;
    return mid + (randomOffset * 16 * sign);
}

