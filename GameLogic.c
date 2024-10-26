// GameLogic.c

#include "GameLogic.h"
#include "Config.h"
#include "Sprite.h"

BOOL CheckCollision(SPRITE* hero, SPRITE* block) {
    if (hero->x < block->x + SPRITE_SIZE &&
        hero->x + SPRITE_SIZE > block->x &&
        hero->y < block->y + SPRITE_SIZE &&
        hero->y + SPRITE_SIZE > block->y) {
        return TRUE;
    }
    return FALSE;
}

void UpdateFrame(SPRITE* HERO, double deltaTime, TRASH trashBlocks[]) {
    BOOL collision = FALSE;
    BOOL onGroundOrBlock = (HERO->y >= 224); // Initially check for ground contact

    // Check key states for continuous movement
    if (GetAsyncKeyState(VK_LEFT) & 0x8000 && !collision) {
        if (HERO->velocityX > -MAXSPEED) {
            HERO->velocityX -= MOVEMENTSPEED;
        }
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && !collision) {
        if (HERO->velocityX < MAXSPEED) {
            HERO->velocityX += MOVEMENTSPEED;
        }
    }
    else {
        HERO->velocityX = 0; // Stop moving if no key is pressed
    }

    // Check for jump input
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && HERO->velocityY == 0) {
        HERO->velocityY = JUMP_STRENGTH; // Initiate jump
    }

    // Apply gravity to the velocity
    HERO->velocityY += GRAVITY;

    // Update HERO's X position based on velocity
    HERO->x += HERO->velocityX;

    // Ensure HERO doesn't go out of bounds horizontally
    if (HERO->x < 0) {
        HERO->x = 0;
        HERO->velocityX = 0;
    }
    else if (HERO->x > 304) {
        HERO->x = 304;
        HERO->velocityX = 0;
    }

    // Check for horizontal collision with TRASH blocks
    for (int i = 0; i < MAX_TRASH_BLOCKS; i++) {
        if (trashBlocks[i].display && CheckCollision(HERO, &trashBlocks[i].sprite)) {
            collision = TRUE;
            // Adjust HERO position to avoid sticking
            if (HERO->velocityX > 0) { // Moving right
                HERO->x = trashBlocks[i].sprite.x - SPRITE_SIZE;
            }
            else if (HERO->velocityX < 0) { // Moving left
                HERO->x = trashBlocks[i].sprite.x + SPRITE_SIZE;
            }
            HERO->velocityX = 0; // Stop horizontal movement
            break;
        }
    }

    // Update HERO's Y position based on velocity
    HERO->y += HERO->velocityY;

    // Check for vertical collision with TRASH blocks
    for (int i = 0; i < MAX_TRASH_BLOCKS; i++) {
        if (trashBlocks[i].display && CheckCollision(HERO, &trashBlocks[i].sprite)) {
            if (HERO->velocityY > 0) { // Moving down
                HERO->y = trashBlocks[i].sprite.y - SPRITE_SIZE;
                HERO->velocityY = 0; // Stop falling
            }
            else if (HERO->velocityY < 0) { // Moving up
                HERO->y = trashBlocks[i].sprite.y + SPRITE_SIZE;
                HERO->velocityY = 0; // Stop ascending
            }
            break;
        }
    }

    // Check for collision with the ground (assuming ground is at y = 224)
    if (HERO->y >= 224) {
        HERO->y = 224;
        HERO->velocityY = 0; // Stop the sprite from falling further
        onGroundOrBlock = TRUE; // HERO is on the ground
    }
}
