#include <windows.h>
#include <cmath>

int main() {
    // Esconde o console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    // Lista de ícones do sistema para alternar
    LPCWSTR icons[] = { IDI_APPLICATION, IDI_HAND, IDI_QUESTION, IDI_EXCLAMATION, IDI_ASTERISK, IDI_WINLOGO, IDI_SHIELD };

    double angle = 0;
    int radius = 100;

    while (true) {
        HDC hdc = GetDC(NULL);

        // Pega a posição atual do mouse para o centro do círculo
        POINT cursor;
        GetCursorPos(&cursor);

        // Calcula a posição X e Y no círculo usando seno e cosseno
        // Usamos LaTeX para a lógica: $x = \cos(\theta) \cdot r$ e $y = \sin(\theta) \cdot r$
        int x = cursor.x + (int)(cos(angle) * radius);
        int y = cursor.y + (int)(sin(angle) * radius);

        // Seleciona um ícone aleatório da lista
        HICON hIcon = LoadIcon(NULL, icons[rand() % 7]);

        // Desenha o ícone na posição calculada
        DrawIcon(hdc, x, y, hIcon);

        // Incrementa o ângulo para o próximo ícone girar
        angle += 0.5;

        // Faz o raio expandir e contrair levemente para um efeito pulsante
        radius = 100 + (int)(sin(angle * 0.1) * 50);

        // Libera o DC
        ReleaseDC(NULL, hdc);

        // Delay para controlar a velocidade da "cobra" de ícones
        Sleep(10);

        // Chance de limpar a tela ocasionalmente
        if (rand() % 100 == 99) {
            InvalidateRect(NULL, NULL, TRUE);
        }
    }

    return 0;
}