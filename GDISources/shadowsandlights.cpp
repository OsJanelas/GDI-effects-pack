#include <iostream>
#include <windows.h>
#include "config.h"

DWORD WINAPI BRIGHT(LPVOID lpParam) {
    while (1) {
        Sleep(40000);
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(0);
        BitBlt(hdc, rand() % 12, rand() % 12, w, h, hdc, rand() % 2, rand() % 2, SRCPAINT);
        Sleep(10);
    }
}

DWORD WINAPI DARK(LPVOID lpParam) {
    HDC hdc;
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        hdc = GetDC(0);
        BitBlt(hdc, rand() % 2, rand() % 12, w, h, hdc, rand() % 12, rand() % 2, SRCAND);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}

int main()
{
    CreateThread(NULL, 0, BRIGHT, NULL, 0, NULL);
    CreateThread(NULL, 0, DARK, NULL, 0, NULL);

    while (1) Sleep(1000);
    return 0;
}
