// Graphics.

#include "Graphics.h"
#include "Config.h"

HDC hdcBackBuffer; // Handle to device context (display settings)				

void InitializeHero(SPRITE* SPRITE, LPCSTR* bitmapArray)
{
    SPRITE->x = 16;
    SPRITE->y = 224;
    SPRITE->frameCount = 3;
    SPRITE->currentFrame = 0;
    SPRITE->velocityY = 0;

    LoadBitmapsToMemory(hdcBackBuffer, bitmapArray, SPRITE->hbitmaps, SPRITE->bitmaps, SPRITE->frameCount);
}

void LoadBitmapsToMemory(HDC hdc, LPCSTR filePaths[], HBITMAP* hBitmaps, BITMAP* bitmaps, int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        hBitmaps[i] = (HBITMAP)LoadImage(NULL, filePaths[i], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        if (hBitmaps[i]) {
            GetObject(hBitmaps[i], sizeof(BITMAP), &bitmaps[i]);
        }
        else {
            MessageBox(NULL, L"Failed to load bitmap!", L"Error", MB_OK);
            PostQuitMessage(0);
        }
    }
}