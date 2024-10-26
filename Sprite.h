// Sprite.h

#ifndef SPRITE_H
#define SPRITE_H

#include <windows.h>

typedef struct SPRITE {
    int x;
    int y;
    HBITMAP hbitmaps[3];
    BITMAP bitmaps[3];
    int currentFrame;
    int frameCount;
    BOOL Display;
    double velocityY;
    double velocityX;
} SPRITE;

extern LPCSTR HEROFilePaths[]; // Declare it as extern
extern LPCSTR TRASHFilePaths[]; // Declare it as extern

#endif // SPRITE_H