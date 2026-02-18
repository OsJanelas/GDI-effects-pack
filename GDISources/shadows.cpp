#include <windows.h>

int main() {
    HDC hdc;
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        hdc = GetDC(0);
        BitBlt(hdc, rand() % 12, rand() % 12, w, h, hdc, rand() % 12, rand() % 12, SRCAND);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}