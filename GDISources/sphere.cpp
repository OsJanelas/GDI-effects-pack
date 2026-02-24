#include <windows.h>
#include <cmath>
#include <vector>
#include <ctime>

#define NUM_POINTS 500
#define SPHERE_RADIUS 150.0f
#define PARTICLE_SIZE 6
#define PI 3.14159265359f

struct Point3D {
    float x, y, z;
};

COLORREF Rainbow(float h) {
    float r, g, b;
    h = fmodf(h, 1.0f);
    float s = 1.0f;
    float v = 1.0f;
    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6) {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
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

        Point3D p;
        p.x = cosf(theta) * sinf(phi);
        p.y = sinf(theta) * sinf(phi);
        p.z = cosf(phi);
        points.push_back(p);
    }

    float angleY = 0;
    float angleX = 0;
    float hue = 0;

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, hbm);

        RECT scrRect = { 0, 0, sw, sh };
        FillRect(hdcMem, &scrRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        int cx = sw / 2;
        int cy = sh / 2;

        angleY += 0.02f;
        angleX += 0.01f;
        hue += 0.005f;

        for (int i = 0; i < NUM_POINTS; i++) {
            float x = points[i].x * SPHERE_RADIUS;
            float y = points[i].y * SPHERE_RADIUS;
            float z = points[i].z * SPHERE_RADIUS;

            float nx = x * cosf(angleY) + z * sinf(angleY);
            float nz = -x * sinf(angleY) + z * cosf(angleY);
            x = nx; z = nz;

            float ny = y * cosf(angleX) - z * sinf(angleX);
            nz = y * sinf(angleX) + z * cosf(angleX);
            y = ny; z = nz;

            int screenX = (int)(x + cx);
            int screenY = (int)(y + cy);

            COLORREF color = Rainbow(hue + (float)i / NUM_POINTS);
            HBRUSH hBrush = CreateSolidBrush(color);
            SelectObject(hdcMem, hBrush);

            int pSize = (int)(PARTICLE_SIZE * (z + SPHERE_RADIUS) / (2 * SPHERE_RADIUS)) + 2;
            Ellipse(hdcMem, screenX - pSize, screenY - pSize, screenX + pSize, screenY + pSize);

            DeleteObject(hBrush);
        }

        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);

        Sleep(10);
    }
    return 0;
}