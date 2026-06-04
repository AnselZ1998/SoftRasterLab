#include <windows.h>
#include <windowsx.h>

#include "Scene.h"

static int gCanvasWidth = 0;
static int gCanvasHeight = 0;

static HDC gWindowCompatibleDC = nullptr;
static HBITMAP gColorBitmap = nullptr;
static unsigned int* gBackgroundColorBuffer = nullptr;
static unsigned short* gBackgroundDepthBuffer = nullptr;

static bool gIsDragging = false;
static int gLastMouseX = 0;
static int gLastMouseY = 0;

static LARGE_INTEGER gFrequency;
static LARGE_INTEGER gPreviousTime;

LRESULT CALLBACK RTWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        OnMouseWheel(delta);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        gIsDragging = true;

        gLastMouseX = GET_X_LPARAM(lParam);
        gLastMouseY = GET_Y_LPARAM(lParam);

        SetCapture(hwnd);
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        if (gIsDragging)
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            int dx = x - gLastMouseX;
            int dy = y - gLastMouseY;

            OnMouseDrag(dx, dy);

            gLastMouseX = x;
            gLastMouseY = y;
        }

        return 0;
    }

    case WM_LBUTTONUP:
    {
        gIsDragging = false;
        ReleaseCapture();
        return 0;
    }

    default:
    {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    }
}

INT WINAPI WinMain(HINSTANCE inHInstance, HINSTANCE, LPSTR inCommandLine, int)
{
    gCanvasWidth = 640;
    gCanvasHeight = 360;

    QueryPerformanceFrequency(&gFrequency);
    QueryPerformanceCounter(&gPreviousTime);

    // Register window class
    WNDCLASSEXW wndclass{};
    wndclass.cbSize = sizeof(WNDCLASSEXW);
    wndclass.style = CS_VREDRAW | CS_HREDRAW;
    wndclass.lpfnWndProc = RTWindowProc;
    wndclass.hInstance = inHInstance;
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass.lpszClassName = L"RTWindow";

    ATOM atom = RegisterClassExW(&wndclass);
    if (!atom)
    {
        return 0;
    }

    RECT rect = { 0, 0, gCanvasWidth, gCanvasHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    int viewportWidth = rect.right - rect.left;
    int viewportHeight = rect.bottom - rect.top;

    HWND hwnd = CreateWindowExW(
        0,
        L"RTWindow",
        L"Alice Rendering Window",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        viewportWidth,
        viewportHeight,
        nullptr,
        nullptr,
        inHInstance,
        nullptr
    );

    if (!hwnd)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    HDC windowCanvasDC = GetDC(hwnd);
    gWindowCompatibleDC = CreateCompatibleDC(windowCanvasDC);

    BITMAPINFO bmpInfo{};
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = gCanvasWidth;
    bmpInfo.bmiHeader.biHeight = gCanvasHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    gColorBitmap = CreateDIBSection(
        windowCanvasDC,
        &bmpInfo,
        DIB_RGB_COLORS,
        reinterpret_cast<void**>(&gBackgroundColorBuffer),
        nullptr,
        0
    );

    SelectObject(gWindowCompatibleDC, gColorBitmap);

    gBackgroundDepthBuffer = new unsigned short[gCanvasWidth * gCanvasHeight];

    InitScene(gCanvasWidth, gCanvasHeight);

    MSG msg{};

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        float deltaTime =
            static_cast<float>(currentTime.QuadPart - gPreviousTime.QuadPart) /
            static_cast<float>(gFrequency.QuadPart);

        gPreviousTime = currentTime;

        RenderOneFrame(deltaTime);

        BitBlt(
            windowCanvasDC,
            0,
            0,
            gCanvasWidth,
            gCanvasHeight,
            gWindowCompatibleDC,
            0,
            0,
            SRCCOPY
        );

        Sleep(1);
    }

    delete[] gBackgroundDepthBuffer;
    gBackgroundDepthBuffer = nullptr;

    if (gColorBitmap)
    {
        DeleteObject(gColorBitmap);
        gColorBitmap = nullptr;
    }

    if (gWindowCompatibleDC)
    {
        DeleteDC(gWindowCompatibleDC);
        gWindowCompatibleDC = nullptr;
    }

    ReleaseDC(hwnd, windowCanvasDC);

    return 0;
}

void SetPixelColor(
    int inX,
    int inY,
    unsigned char inR,
    unsigned char inG,
    unsigned char inB,
    unsigned char inA
)
{
    if (inX < 0 || inX >= gCanvasWidth || inY < 0 || inY >= gCanvasHeight)
    {
        return;
    }

    unsigned int color =
        (static_cast<unsigned int>(inA) << 24) |
        (static_cast<unsigned int>(inR) << 16) |
        (static_cast<unsigned int>(inG) << 8) |
        static_cast<unsigned int>(inB);

    int pixelIndex = inX + inY * gCanvasWidth;
    gBackgroundColorBuffer[pixelIndex] = color;
}