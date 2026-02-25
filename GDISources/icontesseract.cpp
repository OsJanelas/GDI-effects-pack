#include <windows.h>
#include <cmath>
#include <vector>

struct Point4D { float x, y, z, w; };
struct Point2D { int x, y; };

#define PI 3.14159265359f

Point4D Rotate4D(Point4D p, float angle) {
    Point4D target = p;
    float x = target.x * cos(angle) - target.w * sin(angle);
    float w = target.x * sin(angle) + target.w * cos(angle);
    target.x = x; target.w = w;
    float y = target.y * cos(angle * 0.5f) - target.z * sin(angle * 0.5f);
    float z = target.y * sin(angle * 0.5f) + target.z * cos(angle * 0.5f);
    target.y = y; target.z = z;
    return target;
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);

    std::vector<Point4D> vertices;
    for (int i = 0; i < 16; i++) {
        vertices.push_back({
            (i & 1) ? 100.0f : -100.0f,
            (i & 2) ? 100.0f : -100.0f,
            (i & 4) ? 100.0f : -100.0f,
            (i & 8) ? 100.0f : -100.0f
            });
    }

    HICON hIcon = LoadIcon(NULL, IDI_APPLICATION);

    float posX = (float)(sw / 2), posY = (float)(sh / 2);
    float velX = 5.0f, velY = 3.0f, angle = 0.0f;

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, hbm);

        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

        posX += velX; posY += velY;
        if (posX <= 150 || posX + 150 >= sw) velX *= -1;
        if (posY <= 150 || posY + 150 >= sh) velY *= -1;
        angle += 0.02f;

        std::vector<Point2D> projected;
        for (auto& v : vertices) {
            Point4D rotated = Rotate4D(v, angle);

            float distance4D = 200.0f;
            float factor4D = distance4D / (distance4D + rotated.w);

            float distance3D = 400.0f;
            float factor3D = distance3D / (distance3D + (rotated.z * factor4D));

            projected.push_back({
                (int)(rotated.x * factor4D * factor3D + posX),
                (int)(rotated.y * factor4D * factor3D + posY)
                });
        }

        SelectObject(hdcMem, GetStockObject(WHITE_PEN));
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 4; j++) {
                int neighbor = i ^ (1 << j);
                if (neighbor > i) {
                    MoveToEx(hdcMem, projected[i].x, projected[i].y, NULL);
                    LineTo(hdcMem, projected[neighbor].x, projected[neighbor].y);
                }
            }
        }

        for (auto& p : projected) {
            DrawIcon(hdcMem, p.x - 16, p.y - 16, hIcon);
        }

        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
    return 0;
}