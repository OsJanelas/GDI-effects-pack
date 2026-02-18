#include <windows.h>
#include <cmath>

// Estrutura para facilitar a manipulação de cores
struct HSV {
    double h, s, v;
};

// Funções de conversão de cores (necessárias para o seu código funcionar)
namespace Colors {
    HSV RGBtoHSV(RGBQUAD rgb) {
        HSV hsv;
        double r = rgb.rgbRed / 255.0;
        double g = rgb.rgbGreen / 255.0;
        double b = rgb.rgbBlue / 255.0;
        double min = min(r, min(g, b));
        double max = max(r, max(g, b));
        hsv.v = max;
        double delta = max - min;
        if (max > 0.0) hsv.s = delta / max; else hsv.s = 0.0;
        if (hsv.s > 0.0) {
            if (r == max) hsv.h = (g - b) / delta;
            else if (g == max) hsv.h = 2.0 + (b - r) / delta;
            else hsv.h = 4.0 + (r - g) / delta;
            hsv.h *= 60.0;
            if (hsv.h < 0.0) hsv.h += 360.0;
        }
        else hsv.h = 0.0;
        return hsv;
    }

    RGBQUAD HSVtoRGB(HSV hsv) {
        RGBQUAD rgb = { 0 };
        double r, g, b;
        if (hsv.s == 0.0) r = g = b = hsv.v;
        else {
            double h = hsv.h / 60.0;
            int i = (int)h;
            double f = h - i;
            double p = hsv.v * (1.0 - hsv.s);
            double q = hsv.v * (1.0 - hsv.s * f);
            double t = hsv.v * (1.0 - hsv.s * (1.0 - f));
            switch (i) {
            case 0: r = hsv.v; g = t; b = p; break;
            case 1: r = q; g = hsv.v; b = p; break;
            case 2: r = p; g = hsv.v; b = t; break;
            case 3: r = p; g = q; b = hsv.v; break;
            case 4: r = t; g = p; b = hsv.v; break;
            default: r = hsv.v; g = p; b = q; break;
            }
        }
        rgb.rgbRed = (BYTE)(r * 255);
        rgb.rgbGreen = (BYTE)(g * 255);
        rgb.rgbBlue = (BYTE)(b * 255);
        return rgb;
    }
}

int main() {
    // Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    // Configurações baseadas no seu snippet
    int ws = w;
    int hs = h;
    int colorsdelay = 100;

    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);

    BITMAPINFO bmpi = { 0 };
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = ws;
    bmpi.bmiHeader.biHeight = -hs; // Negativo para o bitmap não ficar de ponta cabeça
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(dcCopy, bmp);

    SetStretchBltMode(dc, COLORONCOLOR);
    SetStretchBltMode(dcCopy, COLORONCOLOR);

    while (true) {
        // Captura a tela para o buffer
        StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

        // Escolhe uma matiz aleatória
        int color = rand() % 360;

        // Processa cada pixel (Hue Shift)
        for (int x = 0; x < ws; x++) {
            for (int y = 0; y < hs; y++) {
                int index = y * ws + x;
                HSV hsv = Colors::RGBtoHSV(rgbquad[index]);
                hsv.h = fmod(color, 360.0); // Aplica a nova cor
                rgbquad[index] = Colors::HSVtoRGB(hsv);
            }
        }

        // Delay aleatório conforme seu código
        Sleep(rand() % 500);

        // Desenha de volta na tela
        StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

        // Efeito extra de inversão de padrão (PATINVERT)
        // Nota: PATINVERT usa o Brush atual do DC
        HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 70, rand() % 70, rand() % 70));
        SelectObject(dc, hBrush);
        StretchBlt(dc, 0, 0, w, h, dc, 0, 0, w, h, PATINVERT);

        Sleep(colorsdelay);

        // Limpeza de objetos para não travar o PC
        DeleteObject(hBrush);
        InvalidateRect(NULL, NULL, TRUE); // Simula o System::RedrawScreen()
    }

    return 0;
}