// Config.h

#ifndef CONFIG_H
#define CONFIG_H

#define GAME_NAME L"Trashpile"
#define GAME_WIDTH 320
#define GAME_HEIGHT 240
#define SPRITE_SIZE 16
#define CHAR_SIZE 8
#define FRAME_RATE 120 //60 FPS
#define FRAME_TIME (1000 / FRAME_RATE) // 16.66 ms per frame
#define ANIMATION_INTERVAL 100 // Animation interval in milliseconds
#define JUMP_STRENGTH -10.0
#define GRAVITY 0.5
#define MOVEMENTSPEED 0.5
#define MAXSPEED 2.5
#define FRAME_COUNT 3
#define MAX_TRASH_BLOCKS 300

#endif // CONFIG_H
