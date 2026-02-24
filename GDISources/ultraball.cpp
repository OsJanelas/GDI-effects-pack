#include <windows.h>
#include <cmath>
#include <vector>

struct Point3D { float x, y, z; };
#define NUM_POINTS 600
#define SPHERE_RADIUS 130.0f
#define PI 3.14159265359f

COLORREF GetRainbowColor(float h) {
    float r, g, b;
    h = fmodf(h, 1.0f);
    int i = (int)(h * 6);
    float f = h * 6 - i;
    float q = 1 - f;
    switch (i % 6) {
    case 0: r = 1, g = f, b = 0; break;
    case 1: r = q, g = 1, b = 0; break;
    case 2: r = 0, g = 1, b = f; break;
    case 3: r = 0, g = q, b = 1; break;
    case 4: r = f, g = 0, b = 1; break;
    case 5: r = 1, g = 0, b = q; break;
    }
    return RGB((int)(r * 255), (int)(g * 255), (int)(b * 255));
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);

    std::vector<Point3D> points;
    for (int i = 0; i < NUM_POINTS; i++) {
        float phi = acosf(-1.0f + (2.0f * i) / NUM_POINTS);
        float theta = sqrtf(NUM_POINTS * PI) * phi;
        points.push_back({ cosf(theta) * sinf(phi), sinf(theta) * sinf(phi), cosf(phi) });
    }

    float posX = (float)(rand() % (sw - 300) + 150);
    float posY = (float)(rand() % (sh - 300) + 150);
    float velX = 6.0f;
    float velY = 4.0f;

    float rotX = 0, rotY = 0, hue = 0;

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, hbm);

        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

        posX += velX; posY += velY;
        if (posX - SPHERE_RADIUS <= 0 || posX + SPHERE_RADIUS >= sw) velX *= -1;
        if (posY - SPHERE_RADIUS <= 0 || posY + SPHERE_RADIUS >= sh) velY *= -1;

        rotX += 0.03f; rotY += 0.02f; hue += 0.005f;

        for (int i = 0; i < NUM_POINTS; i++) {
            float x = points[i].x * SPHERE_RADIUS;
            float y = points[i].y * SPHERE_RADIUS;
            float z = points[i].z * SPHERE_RADIUS;

            float ty = y * cosf(rotX) - z * sinf(rotX);
            float tz = y * sinf(rotX) + z * cosf(rotX);
            y = ty; z = tz;

            float tx = x * cosf(rotY) + z * sinf(rotY);
            tz = -x * sinf(rotY) + z * cosf(rotY);
            x = tx; z = tz;

            int screenX = (int)(x + posX);
            int screenY = (int)(y + posY);

            int pSize = (int)((z + SPHERE_RADIUS) / (SPHERE_RADIUS * 2) * 6) + 2;

            if (z > -SPHERE_RADIUS * 0.7) {
                HBRUSH brush = CreateSolidBrush(GetRainbowColor(hue + (float)i / NUM_POINTS));
                SelectObject(hdcMem, brush);

                Ellipse(hdcMem, screenX - pSize, screenY - pSize, screenX + pSize, screenY + pSize);

                DeleteObject(brush);
            }
        }

        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);

        Sleep(10);
    }
    return 0;
}