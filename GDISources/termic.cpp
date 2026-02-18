#include <windows.h>
#include <cmath>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

COLORREF GetHeatColor(float intensity) {
    int r = 0, g = 0, b = 0;

    if (intensity < 0.25f) {
        r = 0; g = (int)(intensity * 4 * 255); b = 255;
    }
    else if (intensity < 0.5f) {
        r = 0; g = 255; b = (int)((0.5f - intensity) * 4 * 255);
    }
    else if (intensity < 0.75f) {
        r = (int)((intensity - 0.5f) * 4 * 255); g = 255; b = 0;
    }
    else {
        r = 255; g = (int)((1.0f - intensity) * 4 * 255); b = (int)((intensity - 0.75f) * 4 * 255);
    }
    return RGB(r, g, b);
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    int scale = 4;
    int ws = w / scale;
    int hs = h / scale;

    HDC hdc = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdc);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ws;
    bmi.bmiHeader.biHeight = -hs;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* pixels;
    HBITMAP hbmp = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
    SelectObject(hdcMem, hbmp);

    while (true) {
        StretchBlt(hdcMem, 0, 0, ws, hs, hdc, 0, 0, w, h, SRCCOPY);

        for (int i = 0; i < ws * hs; i++) {
            float luma = (pixels[i].rgbRed * 0.299f +
                pixels[i].rgbGreen * 0.587f +
                pixels[i].rgbBlue * 0.114f) / 255.0f;

            COLORREF heat = GetHeatColor(luma);

            pixels[i].rgbRed = GetRValue(heat);
            pixels[i].rgbGreen = GetGValue(heat);
            pixels[i].rgbBlue = GetBValue(heat);
        }

        StretchBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, ws, hs, SRCCOPY);

        Sleep(10);
    }

    ReleaseDC(NULL, hdc);
    DeleteDC(hdcMem);
    DeleteObject(hbmp);

    return 0;
}