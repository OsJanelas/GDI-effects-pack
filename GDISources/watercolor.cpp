#include <windows.h>
#include <cmath>
#include <chrono>
#include <thread>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    float timer = 0.0f;
    int gridSize = 20;

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, hbm);

        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

        for (int x = 0; x < sw; x += gridSize) {
            for (int y = 0; y < sh; y += gridSize) {

                float dx = sinf(timer + x * 0.01f) * cosf(timer + y * 0.01f) * 12.0f;
                float dy = cosf(timer + x * 0.01f) * sinf(timer + y * 0.01f) * 12.0f;

                POINT dest[3];
                dest[0].x = (long)(x + dx);          dest[0].y = (long)(y + dy);
                dest[1].x = (long)(x + gridSize + dx); dest[1].y = (long)(y + dy);
                dest[2].x = (long)(x + dx);          dest[2].y = (long)(y + gridSize + dy);

                PlgBlt(hdc, dest, hdcMem, x, y, gridSize, gridSize, NULL, 0, 0);
            }
        }

        timer += 0.05f;

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}