#pragma once

void PlayerSetUp();

typedef struct PLAYER
{
    int x;
    int y;
    HBITMAP hbitmaps[3];  // Array to hold multiple frames
    BITMAP bitmaps[3];    // Array to hold multiple frame bitmaps
    int currentFrame;     // Track the current frame
    int frameCount;       // Total number of frames
    BOOL Display;
} PLAYER;

typedef struct SPRITE
{
    int x;
    int y;
    HBITMAP hbitmap;
    BITMAP bitmap;
    BOOL Display;
} SPRITE;