#pragma once

#include <Windows.h>
#include <cstdint>
#include "Buffer.h"

class DisplayManager
{
public:
    DisplayManager() = default;
    ~DisplayManager() = default;

    bool StartUp(
        HINSTANCE hInstance,
        int clientWidth,
        int clientHeight
    );
    void Present(const Buffer<std::uint32_t>* colorBuffer);
    void ShutDown();

    HWND GetWindowHandle() const;
    int GetWidth() const;
    int GetHeight() const;

private:
    bool RegisterWindowClass();
    bool CreateRenderWindow();
    bool CreateDisplaySurface();

    static LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );

private:
    // 当前 exe 的 Windows 身份
    HINSTANCE mInstance = nullptr;
    // 具体创建出来的窗口
    HWND mWindow = nullptr;

    // 真正的窗口表面
    HDC mWindowDC = nullptr;
    // 内存里的临时表面,先在内存里准备好图片，再一次性复制到窗口
    HDC mMemoryDC = nullptr;

    // Windows 对这张位图的管理句柄
    HBITMAP mBitmap = nullptr;
    // 这张位图真实像素内存的首地址
    HGDIOBJ mPreviousBitmap = nullptr;

    // 终要把自己的 Buffer 数据复制到这里
    std::uint32_t* mBitmapPixels = nullptr;

    int mClientWidth = 0;
    int mClientHeight = 0;

    bool mOwnsWindowClass = false;
};