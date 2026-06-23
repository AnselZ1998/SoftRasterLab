#include "DisplayManager.h"
#include <cassert>
#include <cstring>

namespace
{
    constexpr wchar_t WindowClassName[] =
        L"SoftRasterLabWindowClass";
}

bool DisplayManager::StartUp(
    HINSTANCE hInstance,
    int clientWidth,
    int clientHeight
)
{
    mInstance = hInstance;
    mClientWidth = clientWidth;
    mClientHeight = clientHeight;

    if (!RegisterWindowClass())
    {
        return false;
    }

    if (!CreateRenderWindow())
    {
        ShutDown();
        return false;
    }

    if (clientWidth <= 0 || clientHeight <= 0)
    {
        return false;
    }

    ShowWindow(mWindow, SW_SHOW);
    UpdateWindow(mWindow);

    if (!CreateDisplaySurface())
    {
        ShutDown();
        return false;
    }

    return true;
}

void DisplayManager::Present(const Buffer<std::uint32_t>& colorBuffer)
{
    assert(colorBuffer.GetHeight() == mClientHeight);
    assert(colorBuffer.GetWidth() == mClientWidth);

    if (mBitmapPixels == nullptr)
    {
        return;
    }

    if (colorBuffer.GetWidth() != mClientWidth ||
        colorBuffer.GetHeight() != mClientHeight)
    {
        return;
    }

    std::size_t byteCount =
        colorBuffer.GetPixelCount() *
        sizeof(std::uint32_t);

    std::memcpy(
        mBitmapPixels,
        colorBuffer.Data(),
        byteCount
    );

    BitBlt(
        mWindowDC,
        0,
        0,
        mClientWidth,
        mClientHeight,
        mMemoryDC,
        0,
        0,
        SRCCOPY
    );
}

void DisplayManager::ShutDown()
{
    if (mWindow != nullptr)
    {
        if (IsWindow(mWindow))
        {
            DestroyWindow(mWindow);
        }

        mWindow = nullptr;
    }

    if (mOwnsWindowClass && mInstance != nullptr)
    {
        UnregisterClassW(WindowClassName, mInstance);
        mOwnsWindowClass = false;
    }

    mInstance = nullptr;
}

bool DisplayManager::RegisterWindowClass()
{
    WNDCLASSEXW windowClass{};

    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = mInstance;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszClassName = WindowClassName;

    ATOM result = RegisterClassExW(&windowClass);

    if (result == 0)
    {
        DWORD error = GetLastError();

        if (error != ERROR_CLASS_ALREADY_EXISTS)
        {
            return false;
        }
    }
    else
    {
        mOwnsWindowClass = true;
    }

    return true;
}

bool DisplayManager::CreateRenderWindow()
{
    RECT windowRect{
        0,
        0,
        mClientWidth,
        mClientHeight
    };

    AdjustWindowRect(
        &windowRect,
        WS_OVERLAPPEDWINDOW,
        FALSE
    );

    int windowWidth =
        windowRect.right - windowRect.left;

    int windowHeight =
        windowRect.bottom - windowRect.top;

    mWindow = CreateWindowExW(
        0,
        WindowClassName,
        L"SoftRasterLab",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        windowWidth,
        windowHeight,
        nullptr,
        nullptr,
        mInstance,
        nullptr
    );

    return mWindow != nullptr;
}

bool DisplayManager::CreateDisplaySurface()
{
    mWindowDC = GetDC(mWindow);

    if (mWindowDC == nullptr)
    {
        return false;
    }

    mMemoryDC = CreateCompatibleDC(mWindowDC);

    if (mMemoryDC == nullptr)
    {
        return false;
    }

    BITMAPINFO bitmapInfo{};

    bitmapInfo.bmiHeader.biSize =
        sizeof(BITMAPINFOHEADER);

    bitmapInfo.bmiHeader.biWidth =
        mClientWidth;

    bitmapInfo.bmiHeader.biHeight =
        -mClientHeight;

    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    mBitmap = CreateDIBSection(
        mWindowDC,
        &bitmapInfo,
        DIB_RGB_COLORS,
        reinterpret_cast<void**>(&mBitmapPixels),
        nullptr,
        0
    );

    if (mBitmap == nullptr || mBitmapPixels == nullptr)
    {
        return false;
    }

    mPreviousBitmap = SelectObject(
        mMemoryDC,
        mBitmap
    );

    return true;
}

LRESULT CALLBACK DisplayManager::WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (message)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcW(
            hwnd,
            message,
            wParam,
            lParam
        );
    }
}

HWND DisplayManager::GetWindowHandle() const
{
    return mWindow;
}

int DisplayManager::GetWidth() const
{
    return mClientWidth;
}

int DisplayManager::GetHeight() const
{
    return mClientHeight;
}
