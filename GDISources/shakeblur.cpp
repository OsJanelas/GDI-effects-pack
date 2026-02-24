#include <windows.h>
#include <random>
#include <thread>
#include <chrono>

#pragma comment(lib, "msimg32.lib")

void DistortionEffect() {
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    POINT lppoint[3];

    while (true) {
        HDC hdc = GetDC(nullptr);
        HDC mhdc = CreateCompatibleDC(hdc);
        HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
        HGDIOBJ holdbit = SelectObject(mhdc, hbit);

        if (dis(gen) == 1) {
            lppoint[0] = { 0 + 30, 0 - 30 };
            lppoint[1] = { w + 30, 0 + 30 };
            lppoint[2] = { 0 - 30, h - 30 };
        }
        else {
            lppoint[0] = { 0 - 30, 0 + 30 };
            lppoint[1] = { w - 30, 0 - 30 };
            lppoint[2] = { 0 + 30, h + 30 };
        }

        PlgBlt(mhdc, lppoint, hdc, 0, 0, w, h, nullptr, 0, 0);

        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 60, 0 };
        AlphaBlend(hdc, 0, 0, w, h, mhdc, 0, 0, w, h, blend);

        SelectObject(mhdc, holdbit);
        DeleteObject(hbit);
        DeleteDC(mhdc);
        ReleaseDC(nullptr, hdc);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void ShakeEffect() {
    while (true) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);

        BitBlt(hdc, rand() % 12, rand() % 12, w, h, hdc, rand() % 12, rand() % 12, SRCCOPY);

        ReleaseDC(0, hdc);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void ScreenRefresh() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        InvalidateRect(NULL, NULL, TRUE);
    }
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    std::thread t1(DistortionEffect);
    std::thread t2(ShakeEffect);
    std::thread t3(ScreenRefresh);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}