#include <windows.h>
#include <cmath>

int main() {
    // Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Obtém dimensões da tela
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    HDC hdc = GetDC(NULL);
    HICON hIcon = LoadIcon(NULL, IDI_EXCLAMATION); // Ícone de aviso (!)

    // 1. Define os 3 vértices do triângulo principal
    POINT v[3];
    v[0] = { w / 2, 50 };           // Topo central
    v[1] = { 50, h - 50 };          // Inferior esquerdo
    v[2] = { w - 50, h - 50 };      // Inferior direito

    // Ponto inicial (pode ser qualquer um dos vértices)
    int curX = v[0].x;
    int curY = v[0].y;

    // Loop infinito para desenhar o fractal
    while (true) {
        // 2. Escolhe um dos 3 vértices aleatoriamente
        int target = rand() % 3;

        // 3. Calcula o ponto médio entre a posição atual e o vértice alvo
        // Fórmula: $M = \frac{P_1 + P_2}{2}$
        curX = (curX + v[target].x) / 2;
        curY = (curY + v[target].y) / 2;

        // Desenha o ícone na posição calculada
        // Note: Ícones costumam ter 32x32, então subtraímos 16 para centralizar
        DrawIcon(hdc, curX - 16, curY - 16, hIcon);

        // Delay curtíssimo para o fractal se formar gradualmente
        // Se colocar 0, ele aparece quase instantaneamente (mas consome muita CPU)
        Sleep(1);

        // Lógica de reset: se a tela ficar muito cheia, limpa após 10.000 ícones
        static int count = 0;
        if (++count > 10000) {
            InvalidateRect(NULL, NULL, TRUE);
            count = 0;
            // Opcional: muda os vértices de lugar para variar o desenho
            v[0].x = rand() % w; v[0].y = rand() % h;
            v[1].x = rand() % w; v[1].y = rand() % h;
            v[2].x = rand() % w; v[2].y = rand() % h;
        }
    }

    ReleaseDC(NULL, hdc);
    return 0;
}