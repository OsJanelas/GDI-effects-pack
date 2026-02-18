#include <windows.h>
#include <cmath>

int main() {
    // Esconde a janela do console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Obtém as dimensões da tela
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    // Pega o Device Context da tela inteira
    HDC hdc = GetDC(NULL);

    // Variáveis para as cores e posições
    int r = 255, g = 0, b = 0;
    int state = 0; // Para o ciclo RGB

    while (true) {
        // 1. Ciclo de Cores RGB (Transição Suave)
        if (state == 0) {
            g += 5; if (g >= 255) state = 1;
        }
        else if (state == 1) {
            r -= 5; if (r <= 0) state = 2;
        }
        else if (state == 2) {
            b += 5; if (b >= 255) state = 3;
        }
        else if (state == 3) {
            g -= 5; if (g <= 0) state = 4;
        }
        else if (state == 4) {
            r += 5; if (r >= 255) state = 5;
        }
        else if (state == 5) {
            b -= 5; if (b <= 0) state = 0;
        }

        // 2. Cria uma caneta (Pen) com a cor atual
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(r, g, b));
        SelectObject(hdc, hPen);

        // 3. Define pontos aleatórios para o rabisco
        // MoveToEx define onde a linha começa, LineTo desenha até o fim
        int x1 = rand() % w;
        int y1 = rand() % h;
        int x2 = rand() % w;
        int y2 = rand() % h;

        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);

        // 4. Efeito extra: desenhar um pequeno arco ou círculo aleatório
        if (rand() % 5 == 0) {
            Arc(hdc, x1, y1, x2, y2, rand() % w, rand() % h, rand() % w, rand() % h);
        }

        // Limpeza de memória (Obrigatório para não travar o GDI)
        DeleteObject(hPen);

        // Sleep curto para controlar a densidade do rabisco
        Sleep(5);

        // Chance de limpar a tela para o rabisco recomeçar
        if (rand() % 500 == 499) {
            InvalidateRect(NULL, NULL, TRUE);
        }
    }

    // Nunca chega aqui, mas libera o DC por boa prática
    ReleaseDC(NULL, hdc);
    return 0;
}