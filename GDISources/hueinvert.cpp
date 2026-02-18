#include <windows.h>
#include <cmath>

// Estrutura para HSL
struct HSL {
    float h, s, l;
};

// --- Funções de Conversão HSL para RGB ---
RGBQUAD HSLtoRGB(HSL hsl) {
    float r, g, b;
    float h = hsl.h, s = hsl.s, l = hsl.l;

    if (s == 0) {
        r = g = b = l;
    }
    else {
        auto hue2rgb = [](float p, float q, float t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1.0f / 6) return p + (q - p) * 6 * t;
            if (t < 1.0f / 2) return q;
            if (t < 2.0f / 3) return p + (q - p) * (2.0f / 3 - t) * 6;
            return p;
            };

        float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        r = hue2rgb(p, q, h + 1.0f / 3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f / 3);
    }
    return { (BYTE)(b * 255), (BYTE)(g * 255), (BYTE)(r * 255), 0 };
}

HSL RGBtoHSL(RGBQUAD rgb) {
    float r = rgb.rgbRed / 255.0f, g = rgb.rgbGreen / 255.0f, b = rgb.rgbBlue / 255.0f;
    float max = fmaxf(r, fmaxf(g, b)), min = fminf(r, fminf(g, b));
    float h, s, l = (max + min) / 2.0f;

    if (max == min) {
        h = s = 0;
    }
    else {
        float d = max - min;
        s = l > 0.5f ? d / (2 - max - min) : d / (max + min);
        if (max == r) h = (g - b) / d + (g < b ? 6 : 0);
        else if (max == g) h = (b - r) / d + 2;
        else h = (r - g) / d + 4;
        h /= 6;
    }
    return { h, s, l };
}

// --- Thread 1: Ciclo de Cores HSL Pixel a Pixel ---
DWORD WINAPI HSLEffect(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hdcCopy = CreateCompatibleDC(hdc);

        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = w;
        bmpi.bmiHeader.biHeight = -h; // Inverte para não ficar de cabeça para baixo
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;

        RGBQUAD* rgbquad;
        HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hdcCopy, bmp);

        // Captura a tela
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        // Processa o Hue Shift
        for (int i = 0; i < w * h; i++) {
            HSL hsl = RGBtoHSL(rgbquad[i]);
            hsl.h = fmodf(hsl.h + 0.05f, 1.0f); // Aumentei a velocidade do ciclo
            rgbquad[i] = HSLtoRGB(hsl);
        }

        // Desenha de volta
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);

        // Limpeza
        DeleteObject(bmp);
        DeleteDC(hdcCopy);
        ReleaseDC(NULL, hdc);

        Sleep(10);
    }
}

// --- Thread 2: Inversão de Tela com PatBlt ---
DWORD WINAPI InvertEffect(LPVOID lpParam) {
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);

    while (true) {
        HDC hdc = GetDC(0);
        // PATINVERT inverte as cores com base no Brush selecionado
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

int main() {
    // Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    HANDLE threads[2];
    threads[0] = CreateThread(NULL, 0, HSLEffect, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, InvertEffect, NULL, 0, NULL);

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    return 0;
}