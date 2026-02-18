#include <windows.h>

DWORD WINAPI thing6(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);
        POINT pt[3];
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 55, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);
        DeleteObject(brush);
        DeleteObject(hdcMem); DeleteObject(bm);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}

int main() {
    HANDLE thread = CreateThread(0, 0, thing6, 0, 0, 0);
    Sleep(-1);
}