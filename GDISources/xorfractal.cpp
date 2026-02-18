#include <windows.h>
#include <cmath>

int main() {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    int ws = w / 2;
    int hs = h / 2;

    HDC hdc = GetDC(NULL);
    HDC memDC = CreateCompatibleDC(hdc);

    BITMAPINFO bmpi = { 0 };
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = ws;
    bmpi.bmiHeader.biHeight = -hs;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;

    RGBQUAD* pixels = NULL;
    HBITMAP hBmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
    SelectObject(memDC, hBmp);

    int t = 0;

    while (true) {
        HDC currentDC = GetDC(NULL);

        StretchBlt(memDC, 0, 0, ws, hs, currentDC, 0, 0, w, h, SRCCOPY);

        for (int y = 0; y < hs; y++) {
            for (int x = 0; x < ws; x++) {
                int index = y * ws + x;

                int fractal = (x ^ y ^ t);

                pixels[index].rgbRed ^= (fractal % 255);
                pixels[index].rgbGreen ^= ((fractal * 3) % 255);
                pixels[index].rgbBlue ^= ((fractal * 5) % 255);
            }
        }

        StretchBlt(currentDC, 0, 0, w, h, memDC, 0, 0, ws, hs, SRCINVERT);

        t += 2;

        ReleaseDC(NULL, currentDC);
        Sleep(10);
    }

    return 0;
}

