#include <windows.h>
#include <math.h>
#include <time.h>
#include <vector>

COLORREF palette[] = {
    RGB(255, 2, 2),
    RGB(2, 255, 2),
    RGB(2, 2, 255),
    RGB(2, 255, 255),
    RGB(255, 2, 255),
    RGB(255, 255, 2),
    RGB(255, 255, 255)
};

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    srand((unsigned int)time(NULL));

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    int side = 120;
    float height = side * sqrt(3) / 2;

    while (true) {
        HDC hdc = GetDC(0);
        SetROP2(hdc, R2_XORPEN);

        for (int y = -1; y <= (sh / height) + 1; y++) {
            for (int x = -1; x <= (sw / side) + 1; x++) {

                float offsetX = (y % 2 == 0) ? 0 : side / 2.0f;
                int px = (int)(x * side + offsetX);
                int py = (int)(y * height);

                POINT tri1[3] = {
                    { px, py },
                    { px + side / 2, (int)(py + height) },
                    { px - side / 2, (int)(py + height) }
                };

                POINT tri2[3] = {
                    { px, py },
                    { px + side, py },
                    { px + side / 2, (int)(py + height) }
                };

                HBRUSH b1 = CreateSolidBrush(palette[rand() % 7]);
                HBRUSH b2 = CreateSolidBrush(palette[rand() % 7]);

                SelectObject(hdc, b1);
                Polygon(hdc, tri1, 3);
                DeleteObject(b1);

                SelectObject(hdc, b2);
                Polygon(hdc, tri2, 3);
                DeleteObject(b2);
            }
        }

        ReleaseDC(0, hdc);

        Sleep(1000);

        if (rand() % 5 == 0) InvalidateRect(0, 0, TRUE);
    }

    return 0;
}