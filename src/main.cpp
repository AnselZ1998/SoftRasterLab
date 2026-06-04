#include<windows.h>
#include "Scene.h"

static int gCanvasWidth = 0, gCanvasHeight = 0;
static HDC gWindowCompatibleDC = nullptr;
static HBITMAP gColorBitmap = nullptr;
static unsigned int* gBackgroundColorBuffer = nullptr;
static unsigned short* gBackgroundDepthBuffer = nullptr;
LARGE_INTEGER frequency;
LARGE_INTEGER startTime;
LARGE_INTEGER endTime;

LRESULT CALLBACK RTWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
INT WINAPI WinMain(HINSTANCE inHInstance, HINSTANCE, LPSTR inCommandLine, int) {
    gCanvasWidth = 640;
    gCanvasHeight = 360;

    QueryPerformanceFrequency(&frequency);

    //register window
    WNDCLASSEX wndclass;
    wndclass.cbClsExtra = 0;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.cbWndExtra = 0;
    wndclass.hbrBackground = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hIcon = NULL;
    wndclass.hIconSm = NULL;
    wndclass.hInstance = inHInstance;
    wndclass.lpfnWndProc = RTWindowProc;
    wndclass.lpszClassName = L"RTWindow";
    wndclass.lpszMenuName = NULL;
    wndclass.style = CS_VREDRAW | CS_HREDRAW;
    ATOM atom = RegisterClassEx(&wndclass);
    if (!atom) {
        return 0;
    }
    RECT rect = { 0,0,gCanvasWidth,gCanvasHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    int viewportWidth = rect.right - rect.left;
    int viewportHeight = rect.bottom - rect.top;
    //create window
    HWND hwnd = CreateWindowEx(NULL, L"RTWindow", L"Alice Rendering Window", WS_OVERLAPPEDWINDOW,
        100, 100, viewportWidth, viewportHeight, NULL, NULL, inHInstance, NULL);
    //show window
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    HDC windowCanvasDC = GetDC(hwnd);
    gWindowCompatibleDC = CreateCompatibleDC(windowCanvasDC);

    BITMAPINFO	bmpInfor;
    bmpInfor.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfor.bmiHeader.biWidth = gCanvasWidth;
    bmpInfor.bmiHeader.biHeight = gCanvasHeight;
    bmpInfor.bmiHeader.biPlanes = 1;
    bmpInfor.bmiHeader.biBitCount = 32;
    bmpInfor.bmiHeader.biCompression = BI_RGB;
    bmpInfor.bmiHeader.biSizeImage = 0;
    bmpInfor.bmiHeader.biXPelsPerMeter = 0;
    bmpInfor.bmiHeader.biYPelsPerMeter = 0;
    bmpInfor.bmiHeader.biClrUsed = 0;
    bmpInfor.bmiHeader.biClrImportant = 0;

    gColorBitmap = CreateDIBSection(windowCanvasDC, &bmpInfor, DIB_RGB_COLORS, (void**)&gBackgroundColorBuffer, 0, 0);
    SelectObject(gWindowCompatibleDC, gColorBitmap);
    gBackgroundDepthBuffer = new unsigned short[gCanvasWidth * gCanvasHeight];
    InitScene(gCanvasWidth, gCanvasHeight);
    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        QueryPerformanceCounter(&startTime);
        //debug mode
        //RenderOneFrame(0.0f);
        QueryPerformanceCounter(&endTime);
        float deltaTime = (float)(endTime.QuadPart - startTime.QuadPart) / (float)frequency.QuadPart;
        //std::cout << "deltaTime: " << deltaTime  << "s" << std::endl;
        BitBlt(windowCanvasDC, 0, 0, gCanvasWidth, gCanvasHeight, gWindowCompatibleDC, 0, 0, SRCCOPY);
        Sleep(1);
    }
}
void SetPixelColor(int inX, int inY, unsigned char inR, unsigned char inG, unsigned char inB, unsigned char inA) {
    unsigned int color = (inA << 24) + (inR << 16) + (inG << 8) + inB;
    int nPixelCount = gCanvasWidth * gCanvasHeight;
    int nPixelIndex = inX + inY * gCanvasWidth;
    if (nPixelIndex < nPixelCount) {
        gBackgroundColorBuffer[nPixelIndex] = color;
    }
}