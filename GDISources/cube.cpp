//THIS CODE IS TO MAKE A 3D CUBE SIMILAR WITH DDT+ CUBE

#include <windows.h>
#include <cmath>
#include <vector>

struct Point3D {
    float x, y, z;
};

struct Point2D {
    int x, y;
};

#define SIZE 100.0f
#define PI 3.14159265359f

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    float posX = (float)(rand() % (sw - 200));
    float posY = (float)(rand() % (sh - 200));
    float velX = 5.0f;
    float velY = 3.0f;

    float angleX = 0, angleY = 0, angleZ = 0;

    Point3D vertices[8] = {
        {-SIZE, -SIZE,  SIZE}, {SIZE, -SIZE,  SIZE}, {SIZE,  SIZE,  SIZE}, {-SIZE,  SIZE,  SIZE},
        {-SIZE, -SIZE, -SIZE}, {SIZE, -SIZE, -SIZE}, {SIZE,  SIZE, -SIZE}, {-SIZE,  SIZE, -SIZE}
    };

    // FACES
    int faces[6][4] = {
        {0, 1, 2, 3}, {1, 5, 6, 2}, {5, 4, 7, 6}, {4, 0, 3, 7}, {3, 2, 6, 7}, {4, 5, 1, 0}
    };

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, hbm);

        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

        // POSITION
        posX += velX; posY += velY;
        if (posX <= 150 || posX + 150 >= sw) velX *= -1;
        if (posY <= 150 || posY + 150 >= sh) velY *= -1;

        angleX += 0.05f; angleY += 0.03f; angleZ += 0.02f;

        std::vector<Point2D> projected(8);

        // ROTATING
        for (int i = 0; i < 8; i++) {
            float x = vertices[i].x;
            float y = vertices[i].y;
            float z = vertices[i].z;

            // ROTATING X
            float ny = y * cos(angleX) - z * sin(angleX);
            float nz = y * sin(angleX) + z * cos(angleX);
            y = ny; z = nz;

            // ROTATING Y
            float nx = x * cos(angleY) + z * sin(angleY);
            nz = -x * sin(angleY) + z * cos(angleY);
            x = nx; z = nz;

            // PERSPECTIVE
            float dist = 400.0f;
            float factor = dist / (dist + z);
            projected[i].x = (int)(x * factor + posX);
            projected[i].y = (int)(y * factor + posY);
        }

        // DRAW FACES
        for (int i = 0; i < 6; i++) {
            POINT p[4] = {
                {projected[faces[i][0]].x, projected[faces[i][0]].y},
                {projected[faces[i][1]].x, projected[faces[i][1]].y},
                {projected[faces[i][2]].x, projected[faces[i][2]].y},
                {projected[faces[i][3]].x, projected[faces[i][3]].y}
            };

            // SHADOWS
            HBRUSH brush = CreateSolidBrush(RGB(100 + (i * 20), 100 + (i * 20), 100 + (i * 20)));
            SelectObject(hdcMem, brush);
            SelectObject(hdcMem, GetStockObject(BLACK_PEN));

            // DRAW
            Polygon(hdcMem, p, 4);

            DeleteObject(brush);
        }

        BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);

        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdc);

        Sleep(16); // 60 FPS
    }
    return 0;

}
