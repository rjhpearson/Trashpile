#include <windows.h>
#include "Animations.c"

#define GAME_NAME L"Trashpile"
#define GAME_WIDTH 320
#define GAME_HEIGHT 240
#define SPRITE_SIZE 16 // Define the sprite size for rendering
#define FRAME_RATE 120 //60                   // 60 FPS
#define FRAME_TIME (1000 / FRAME_RATE)  // 16.66 Frames
#define ANIMATION_INTERVAL 100 // Animation interval in milliseconds
#define GRAVITY 0.5
#define MOVEMENTSPEED 0.5
#define MAXSPEED 5
#define JUMP_STRENGTH -10.0

int GAMESTATE = 0; // Start with the intro

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadBitmapsToMemory(HDC hdc, LPCSTR filePaths[], HBITMAP* hBitmaps, BITMAP* bitmaps, int frameCount);
void UpdateFrame(double deltaTime);
void RenderFrame(HWND hwnd);
void UpdateAnimationFrames();

typedef struct SPRITE
{
    int x;
    int y;
    HBITMAP hbitmaps[3];    // Array to hold multiple frames
    BITMAP bitmaps[3];      // Array to hold multiple frame bitmaps
    int currentFrame;       // Track the current frame
    int frameCount;         // Total number of frames
    BOOL Display;           // Display Sprite
    double velocityY;       // Y velocity for gravity, initially set to 0
    double velocityX;       // X velocity for controlling player movement
} SPRITE;

SPRITE HERO, HERO;

// Backbuffer
HDC hdcBackBuffer;
HBITMAP hbmBackBuffer;

// Variables for the intro state
HBITMAP hbmIntro;
BITMAP bmIntro;
double introY = 0; // Start the intro bitmap at top of screen
double introScrollSpeed = 5;  // Speed at which the intro scrolls

DWORD lastAnimationTime = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {

    HERO.x = 16;
    HERO.y = 224;
    HERO.velocityY = 0;
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

    // Load the HERO's animation frames
    LPCSTR HEROFilePaths[] = {
        L"C:\\Users\\rjhpe\\OneDrive\\Desktop\\hero1.bmp",
        L"C:\\Users\\rjhpe\\OneDrive\\Desktop\\hero2.bmp",
        L"C:\\Users\\rjhpe\\OneDrive\\Desktop\\hero3.bmp"
    };
    HERO.frameCount = 3;
    HERO.currentFrame = 0;
    LoadBitmapsToMemory(hdcBackBuffer, HEROFilePaths, HERO.hbitmaps, HERO.bitmaps, HERO.frameCount);

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
            UpdateFrame(deltaTime);
            UpdateAnimationFrames();  // Update animation frames based on the timer
            RenderFrame(hwnd);
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

void UpdateFrame(double deltaTime) {

    // Check key states for continuous movement
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (HERO.velocityX > -MAXSPEED) {
            HERO.velocityX -= MOVEMENTSPEED;
        }
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (HERO.velocityX < MAXSPEED) {
            HERO.velocityX += MOVEMENTSPEED;
        }
    }
    else {
        HERO.velocityX = 0; // Stop moving if no key is pressed
    }

    // Apply gravity to the velocity
    HERO.velocityY += GRAVITY; // *deltaTime;

    // Update HERO's X position based on velocity
    HERO.x += HERO.velocityX; // *deltaTime;

    // Update HERO's Y position based on velocity
    HERO.y += HERO.velocityY; // *deltaTime;

    // Check for collision with the ground (assuming ground is at y = 224)
    if (HERO.y >= 224) {
        HERO.y = 224;
        HERO.velocityY = 0; // Stop the sprite from falling further
    }
    
}

// Used to update sprint Animations
void UpdateAnimationFrames() {
    DWORD currentTime = GetTickCount64();

    if (currentTime - lastAnimationTime >= ANIMATION_INTERVAL) {
        // Update HERO animation frame
        HERO.currentFrame = (HERO.currentFrame + 1) % HERO.frameCount;

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
