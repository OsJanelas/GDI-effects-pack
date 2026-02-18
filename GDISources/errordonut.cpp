#include <windows.h>
#include <cmath>

// Constantes matemáticas
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    // Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    HDC hdc = GetDC(NULL);
    HICON hIcon = LoadIcon(NULL, IDI_HAND); // Ícone de Erro (X vermelho)

    // Variáveis do Donut
    float A = 0, B = 0; // Ângulos de rotação
    float i, j;

    // Variáveis do Movimento (DVD Style)
    int posX = screenW / 2, posY = screenH / 2;
    int velX = 15, velY = 15;
    int donutSize = 150; // Raio aproximado do donut na tela

    while (true) {
        // 1. Lógica de movimento "DVD"
        posX += velX;
        posY += velY;

        // Colisão com as bordas
        if (posX + donutSize >= screenW || posX - donutSize <= 0) velX = -velX;
        if (posY + donutSize >= screenH || posY - donutSize <= 0) velY = -velY;

        // 2. Desenho do Donut
        // i gira em torno do círculo menor do tubo, j gira em torno do centro do donut
        for (j = 0; j < 6.28; j += 0.4) {
            for (i = 0; i < 6.28; i += 0.2) {

                // Equações paramétricas do Donut projetadas em 2D
                float sinA = sin(A), cosA = cos(A);
                float sinB = sin(B), cosB = cos(B);
                float sini = sin(i), cosi = cos(i);
                float sinj = sin(j), cosj = cos(j);

                float h = cosi + 2; // 2 é o raio do anel central
                float mess = 1 / (sini * sinA * h + sinj * cosA + 5); // Profundidade
                float t = sini * cosA * h - sinj * sinA;

                // Projeção final X e Y
                int x = posX + (int)(30 * mess * (cosi * cosA * h - t * sinA));
                int y = posY + (int)(15 * mess * (cosi * sinB * h + t * cosB));

                // Desenha o ícone de erro na posição projetada
                DrawIcon(hdc, x, y, hIcon);
            }
        }

        // 3. Atualiza os ângulos de rotação do donut
        A += 0.08;
        B += 0.04;

        // Libera um pouco de CPU e limpa rastro (opcional)
        Sleep(10);

        // Redraw ocasional para não deixar a tela totalmente coberta
        if (rand() % 20 == 0) {
            InvalidateRect(NULL, NULL, TRUE);
        }
    }

    ReleaseDC(NULL, hdc);
    return 0;
}