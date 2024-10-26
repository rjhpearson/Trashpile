// Main.h

#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include "Config.h" // Include the new config header

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void RenderFrame(HWND hwnd);
void UpdateAnimationFrames();

// Variables
HBITMAP hbmBackBuffer;
HBITMAP hbmIntro;
BITMAP bmIntro;
double introY = 0;
double introScrollSpeed = 1;
DWORD lastAnimationTime = 0;

#endif // MAIN_H
