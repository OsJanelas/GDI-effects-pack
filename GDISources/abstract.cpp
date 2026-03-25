#include <windows.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>

typedef struct {
    FLOAT h;
    FLOAT s;
    FLOAT l;
} HSL;

COLORREF HSLtoRGB(HSL hsl) {
    float r, g, b;
    float h = hsl.h;
    float s = hsl.s;
    float l = hsl.l;

    if (s == 0) {
        r = g = b = l;
    }
    else {
        auto hue2rgb = [](float p, float q, float t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1.0f / 6.0f) return p + (q - p) * 6 * t;
            if (t < 1.0f / 2.0f) return q;
            if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6;
            return p;
            };
        float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        r = hue2rgb(p, q, h + 1.0f / 3.0f);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f / 3.0f);
    }
    return RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}

struct MeshPoint {
    POINT pt;
    float targetHue;
    float currentHue;
};

const int GRID_SIZE_X = 20;
const int GRID_SIZE_Y = 15;
const float COLOR_STEP = 0.005f;

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    srand((unsigned int)time(NULL));

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    std::vector<std::vector<MeshPoint>> mesh(GRID_SIZE_Y + 1, std::vector<MeshPoint>(GRID_SIZE_X + 1));
    int cellW = sw / GRID_SIZE_X;
    int cellH = sh / GRID_SIZE_Y;

    for (int y = 0; y <= GRID_SIZE_Y; y++) {
        for (int x = 0; x <= GRID_SIZE_X; x++) {
            mesh[y][x].pt.x = x * cellW;
            mesh[y][x].pt.y = y * cellH;

            if (x > 0 && x < GRID_SIZE_X) mesh[y][x].pt.x += (rand() % cellW) - (cellW / 2);
            if (y > 0 && y < GRID_SIZE_Y) mesh[y][x].pt.y += (rand() % cellH) - (cellH / 2);

            float normX = (float)x / GRID_SIZE_X;
            float normY = (float)y / GRID_SIZE_Y;

            float hue;
            if (normX < 0.25f) {
                hue = 0.05f - (normX * 0.2f);
                if (hue < 0) hue += 1.0f;
            }
            else if (normX < 0.5f) {
                hue = 1.0f - ((normX - 0.25f) * 0.8f);
            }
            else {
                hue = 0.8f - ((normX - 0.5f) * 0.4f);
            }

            mesh[y][x].currentHue = hue;
            mesh[y][x].targetHue = hue;
        }
    }

    float globalHueShift = 0.0f;

    while (true) {
        HDC hdc = GetDC(0);

        for (int y = 0; y < GRID_SIZE_Y; y++) {
            for (int x = 0; x < GRID_SIZE_X; x++) {

                POINT pTL = mesh[y][x].pt;
                POINT pTR = mesh[y][x + 1].pt;
                POINT pBL = mesh[y + 1][x].pt;
                POINT pBR = mesh[y + 1][x + 1].pt;

                POINT tri1[3] = { pTL, pTR, pBL };

                HSL color1;
                color1.h = fmod(mesh[y][x].currentHue + globalHueShift, 1.0f);
                color1.s = 0.7f;
                color1.l = 0.5f + ((float)y / GRID_SIZE_Y * 0.2f);

                HBRUSH brush1 = CreateSolidBrush(HSLtoRGB(color1));
                HPEN pen1 = CreatePen(PS_SOLID, 1, HSLtoRGB(color1));

                SelectObject(hdc, brush1);
                SelectObject(hdc, pen1);
                Polygon(hdc, tri1, 3);

                DeleteObject(brush1);
                DeleteObject(pen1);

                POINT tri2[3] = { pTR, pBR, pBL };

                HSL color2;
                color2.h = fmod(mesh[y][x + 1].currentHue + globalHueShift, 1.0f);
                color2.s = 0.7f;
                color2.l = 0.5f + ((float)(y + 1) / GRID_SIZE_Y * 0.2f);

                HBRUSH brush2 = CreateSolidBrush(HSLtoRGB(color2));
                HPEN pen2 = CreatePen(PS_SOLID, 1, HSLtoRGB(color2));

                SelectObject(hdc, brush2);
                SelectObject(hdc, pen2);
                Polygon(hdc, tri2, 3);

                DeleteObject(brush2);
                DeleteObject(pen2);
            }
        }

        globalHueShift += COLOR_STEP;
        if (globalHueShift > 1.0f) globalHueShift = 0.0f;

        ReleaseDC(0, hdc);

        Sleep(100);
    }

    return 0;
}