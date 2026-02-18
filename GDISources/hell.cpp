#include <windows.h>
#include <math.h>
#include <iostream>

// --- Estruturas e Conversões de Cores ---
typedef struct { double h, s, v; } HSV;

RGBQUAD HSVtoRGB(HSV hsv) {
    double r, g, b;
    int i = (int)floor(hsv.h / 60.0) % 6;
    double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
    double p = hsv.v * (1.0 - hsv.s);
    double q = hsv.v * (1.0 - f * hsv.s);
    double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);
    switch (i) {
    case 0: r = hsv.v, g = t, b = p; break;
    case 1: r = q, g = hsv.v, b = p; break;
    case 2: r = p, g = hsv.v, b = t; break;
    case 3: r = p, g = q, b = hsv.v; break;
    case 4: r = t, g = p, b = hsv.v; break;
    case 5: r = hsv.v, g = p, b = q; break;
    }
    return { (BYTE)(b * 255), (BYTE)(g * 255), (BYTE)(r * 255), 0 };
}

HSV RGBtoHSV(RGBQUAD rgb) {
    double r = rgb.rgbRed / 255.0, g = rgb.rgbGreen / 255.0, b = rgb.rgbBlue / 255.0;
    double maxC = max(r, max(g, b)), minC = min(r, min(g, b));
    double delta = maxC - minC;
    HSV hsv = { 0, 0, maxC };
    if (delta > 0) {
        if (maxC == r) hsv.h = fmod((g - b) / delta, 6.0);
        else if (maxC == g) hsv.h = (b - r) / delta + 2.0;
        else hsv.h = (r - g) / delta + 4.0;
        hsv.h *= 60.0; if (hsv.h < 0) hsv.h += 360.0;
        hsv.s = delta / maxC;
    }
    return hsv;
}

// --- Efeito 1: Bola Saltitante RGB ---
DWORD WINAPI BouncingBall(LPVOID lpParam) {
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    int x = 10, y = 10, signX = 1, signY = 1;
    while (1) {
        HDC hdc = GetDC(0);
        x += 10 * signX; y += 10 * signY;
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        Ellipse(hdc, x, y, x + 100, y + 100);
        if (y >= sh - 100 || y <= 0) signY *= -1;
        if (x >= sw - 100 || x <= 0) signX *= -1;
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

// --- Efeito 2: Plasma HSV Dinâmico ---
DWORD WINAPI RandomPlasma(LPVOID lpvd) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);
    int ws = w / 4, hs = h / 4;
    BITMAPINFO bmpi = { sizeof(BITMAPINFOHEADER), ws, hs, 1, 32, BI_RGB };
    RGBQUAD* rgbquad = NULL;
    HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(dcCopy, bmp);
    for (int i = 0; ; i++) {
        StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);
        int randx = rand() % ws, randy = rand() % hs;
        for (int x = 0; x < ws; x++) {
            for (int y = 0; y < hs; y++) {
                int index = y * ws + x;
                float zx = pow(x - randx, 2) / 25.0;
                float zy = pow(y - randy, 2) / 9.0;
                int fx = 128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0));
                HSV hsv = RGBtoHSV(rgbquad[index]);
                hsv.h = fmod(fx + i, 360.0);
                rgbquad[index] = HSVtoRGB(hsv);
            }
        }
        StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
        Sleep(100);
    }
}

// --- Efeito 3: Pixel Stretching Vertical ---
DWORD WINAPI PixelStretch(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        int x = rand() % w;
        BitBlt(hdc, x, 5, 10, h, hdc, x, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(2);
    }
}

// --- Efeito 4: Inversão de Tela (Flash) ---
DWORD WINAPI ScreenInvert(LPVOID lpParam) {
    int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

// --- Efeito 5: Glitch Memory BitBlt (thing6) ---
DWORD WINAPI Thing6(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);

        // Aplica o BitBlt com a ROP customizada 0x123456
        BitBlt(hdc, rand() % 5, rand() % 5, sw, sh, hdcMem, rand() % 5, rand() % 5, 0x123456);

        DeleteObject(brush);
        DeleteObject(bm);
        DeleteDC(hdcMem);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}

int main() {
    // Esconde o console para o efeito ser total
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Array de Handles para gerenciar as threads
    HANDLE threads[5];

    threads[0] = CreateThread(NULL, 0, BouncingBall, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, RandomPlasma, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, PixelStretch, NULL, 0, NULL);
    threads[3] = CreateThread(NULL, 0, ScreenInvert, NULL, 0, NULL);
    threads[4] = CreateThread(NULL, 0, Thing6, NULL, 0, NULL);

    // Mantém o programa rodando enquanto as threads estiverem ativas
    WaitForMultipleObjects(5, threads, TRUE, INFINITE);

    return 0;
}