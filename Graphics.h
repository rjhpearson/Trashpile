// Graphics.h

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include "Sprite.h"
#include "Config.h"

extern HDC hdcBackBuffer;

void InitializeHero(SPRITE* SPRITE, LPCSTR* bitmapArray);
void LoadBitmapsToMemory(HDC hdc, LPCSTR filePaths[], HBITMAP* hBitmaps, BITMAP* bitmaps, int frameCount);

SPRITE HERO;

#endif // GRAPHICS_H