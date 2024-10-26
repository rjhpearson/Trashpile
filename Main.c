// Main.c

#include <windows.h>
#include "Main.h"
#include "GameLogic.h"
#include "Graphics.h"
#include "TrashBlock.h"

int GAMESTATE = 0; // Start with the intro

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {

    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = GAME_NAME;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), GAME_NAME, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(GAME_NAME, GAME_NAME,
        WS_POPUP, // Full screen style
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    HDC hdc = GetDC(hwnd);

    // Create the back buffer
    hdcBackBuffer = CreateCompatibleDC(hdc);
    hbmBackBuffer = CreateCompatibleBitmap(hdc, GAME_WIDTH, GAME_HEIGHT);
    SelectObject(hdcBackBuffer, hbmBackBuffer);
    ReleaseDC(hwnd, hdc);

    // Load Sprites
    InitializeHero(&HERO, HEROFilePaths);

    // Initialise Trash Blocks
    InitializeTrashBlocks();

    // Load the intro bitmap
    hbmIntro = (HBITMAP)LoadImage(NULL, L"C:\\Users\\rjhpe\\OneDrive\\Desktop\\intro.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hbmIntro) {
        GetObject(hbmIntro, sizeof(BITMAP), &bmIntro);
    }
    else {
        MessageBox(NULL, L"Failed to load intro bitmap!", L"Error", MB_OK);
        PostQuitMessage(0);
    }

    lastAnimationTime = GetTickCount64();  // Initialize the timer for animation

    DWORD lastFrameTime = GetTickCount64();
    DWORD currentTime;
    DWORD deltaTime;

    while (1) {
        // Process all pending messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                return (int)msg.wParam;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        currentTime = GetTickCount64();
        deltaTime = currentTime - lastFrameTime;

        if (deltaTime >= FRAME_TIME) {
            lastFrameTime = currentTime;
            UpdateAnimationFrames();  // Update animation frames based on the timer
            RenderFrame(hwnd);
            if (GAMESTATE == 1)
            {
                UpdateFrame(&HERO, deltaTime, trashBlocks);
                UpdateTrashBlocks(deltaTime);
            }
        }
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_UP:
            if (HERO.y != 0) {
                HERO.y -= 16;
            }
            break;
        case VK_DOWN:
            if (HERO.y != 224) {
                HERO.y += 16;
            }
            break;
            // Skip the intro and start the game
        case 'G':
            if (GAMESTATE == 0) {
                GAMESTATE = 1;
            }
            break;
        case VK_ESCAPE:
            PostQuitMessage(WM_QUIT);
            break;
        case VK_SPACE:
            if (HERO.y >= 224) {  // Ensure the sprite can only jump if it's on the ground
                HERO.velocityY = JUMP_STRENGTH;
            }
            break;
        }
        return 0;
        case WM_KEYUP:
        switch (wParam) {
            case VK_LEFT:
            case VK_RIGHT:
                HERO.velocityX = 0;
                break;
        }
        return 0;
    case WM_DESTROY:
        for (int i = 0; i < HERO.frameCount; i++) {
            DeleteObject(HERO.hbitmaps[i]);
        }
        DeleteObject(hbmIntro); // Clean up the intro bitmap
        DeleteDC(hdcBackBuffer);
        DeleteObject(hbmBackBuffer);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}


// Used to update sprint Animations
void UpdateAnimationFrames() {
    DWORD currentTime = GetTickCount64();

    if (currentTime - lastAnimationTime >= ANIMATION_INTERVAL) {
        // Update HERO animation frame
        if (HERO.frameCount > 0) {
            HERO.currentFrame = (HERO.currentFrame + 1) % HERO.frameCount;
        }
        lastAnimationTime = currentTime;
    }

}

// Used to render the graphics
void RenderFrame(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    HDC hdcMem = CreateCompatibleDC(hdcBackBuffer);

    // Clear the back buffer (game area)
    BitBlt(hdcBackBuffer, 0, 0, GAME_WIDTH, GAME_HEIGHT, NULL, 0, 0, BLACKNESS);

    switch (GAMESTATE) {
    case 0:
        // Draw the intro bitmap scrolling from top to bottom
        SelectObject(hdcMem, hbmIntro);
        BitBlt(hdcBackBuffer, 0, introY, bmIntro.bmWidth, bmIntro.bmHeight, hdcMem, 0, 0, SRCCOPY);

        // Update the intro position
        if (introY > -240)
        {
            introY -= introScrollSpeed;
        }

        break;
    case 1:
        // Draw the current HERO frame at its position
        SelectObject(hdcMem, HERO.hbitmaps[HERO.currentFrame]);
        BitBlt(hdcBackBuffer, HERO.x, HERO.y, SPRITE_SIZE, SPRITE_SIZE, hdcMem, 0, 0, SRCCOPY);

        //Render Trash Blocks
        RenderTrashBlocks(hdcBackBuffer);
        break;
    case 2:
        break;
    }

    // Stretch the back buffer to the full screen
    StretchBlt(hdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        hdcBackBuffer, 0, 0, GAME_WIDTH, GAME_HEIGHT, SRCCOPY);

    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);
}
