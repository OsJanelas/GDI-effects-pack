#include <windows.h>
#include <iostream>
#include <cmath>

#pragma comment(lib, "Msimg32.lib")

// Constante para o valor de PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    // Esconde o console para o efeito ficar "limpo"
    FreeConsole();

    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    HDC hdc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
    SelectObject(dcCopy, bmp);

    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 15, 0 };
    POINT rotationPoints[3];
    double angle = 0;

    while (true) {
        // 1. Atualiza o ângulo de rotação
        angle += 0.05;
        if (angle > 2 * M_PI) angle = 0;

        // 2. Calcula os pontos do paralelogramo para rotacionar a tela
        // O PlgBlt precisa de 3 pontos: Superior-Esquerdo, Superior-Direito e Inferior-Esquerdo
        float cosA = cos(angle) * 0.02; // Multiplicador baixo para não "fugir" da tela
        float sinA = sin(angle) * 0.02;

        rotationPoints[0].x = rand() % 10;
        rotationPoints[0].y = rand() % 10;
        rotationPoints[1].x = w - rand() % 10;
        rotationPoints[1].y = rand() % 10;
        rotationPoints[2].x = rand() % 10;
        rotationPoints[2].y = h - rand() % 10;

        // 3. Efeito de Glitch/Stretch do primeiro código
        StretchBlt(dcCopy, rand() % 15, rand() % 15, w, h, hdc, rand() % -15, rand() % -15, w, h, SRCCOPY);

        // 4. Aplica a rotação usando PlgBlt
        PlgBlt(hdc, rotationPoints, hdc, 0, 0, w, h, NULL, 0, 0);

        // 5. Efeito de transparência (AlphaBlend)
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);

        // 6. Pequeno delay para não travar o processador 100%
        Sleep(10);

        // Reset ocasional para evitar que a tela fique preta ou branca demais
        if (rand() % 50 == 0) {
            InvalidateRect(NULL, NULL, TRUE);
        }
    }

    // Limpeza (nunca chega aqui no loop infinito, mas boa prática)
    ReleaseDC(NULL, hdc);
    DeleteDC(dcCopy);
    DeleteObject(bmp);

    return 0;
}