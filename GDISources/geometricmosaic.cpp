#include <windows.h>
#include <vector>
#include <ctime>

#define TILE_SIZE 100

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    srand((unsigned int)time(NULL));

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    while (true) {
        HDC hdc = GetDC(NULL);

        for (int x = 0; x < sw; x += TILE_SIZE) {
            for (int y = 0; y < sh; y += TILE_SIZE) {

                HBRUSH brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
                SelectObject(hdc, brush);
                SelectObject(hdc, GetStockObject(NULL_PEN));

                POINT tri1[3] = { {x, y}, {x + TILE_SIZE, y}, {x, y + TILE_SIZE} };
                Polygon(hdc, tri1, 3);

                DeleteObject(brush);
                brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
                SelectObject(hdc, brush);

                POINT tri2[3] = { {x + TILE_SIZE, y}, {x + TILE_SIZE, y + TILE_SIZE}, {x, y + TILE_SIZE} };
                Polygon(hdc, tri2, 3);

                DeleteObject(brush);
            }
        }

        Sleep(500);

        ReleaseDC(NULL, hdc);
    }

    return 0;
}