#include <windows.h>
#include <vector>
#include <cmath>
#include <ctime>

#pragma comment(lib, "msimg32.lib")

struct PolygonShape {
    POINT points[4];
    int numPoints;
    COLORREF color;
    float angle;
    float rotationSpeed;
    int distance;
};

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    srand((unsigned int)time(NULL));

    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);

    std::vector<PolygonShape> shapes;
    for (int i = 0; i < 15; i++) {
        PolygonShape s;
        s.numPoints = (rand() % 2 == 0) ? 3 : 4;
        s.color = RGB(rand() % 255, rand() % 255, rand() % 255);
        s.angle = (float)(rand() % 360);
        s.rotationSpeed = (float)((rand() % 10) + 2) / 100.0f;
        s.distance = (rand() % 80) + 20;
        shapes.push_back(s);
    }

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, hbm);

        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

        POINT mouse;
        GetCursorPos(&mouse);

        for (auto& s : shapes) {
            s.angle += s.rotationSpeed;

            POINT drawPoints[4];
            int size = 30;

            for (int j = 0; j < s.numPoints; j++) {
                float vertexAngle = s.angle + (j * (2 * 3.14159f / s.numPoints));
                drawPoints[j].x = mouse.x + (long)(cos(vertexAngle) * s.distance);
                drawPoints[j].y = mouse.y + (long)(sin(vertexAngle) * s.distance);
            }

            HBRUSH brush = CreateSolidBrush(s.color);
            SelectObject(hdcMem, brush);
            SelectObject(hdcMem, GetStockObject(NULL_PEN));

            Polygon(hdcMem, drawPoints, s.numPoints);
            DeleteObject(brush);
        }

        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 127, 0 };
        AlphaBlend(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, blend);

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);

        Sleep(10);
    }
    return 0;
}