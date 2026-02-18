#include <windows.h>

#include <iostream>

#pragma comment(lib, "Msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main() {
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);
    HBITMAP bmp = CreateCompatibleBitmap(dc, w, h);
    SelectObject(dcCopy, bmp);

    BLENDFUNCTION blur = { AC_SRC_OVER, 0, 10, 0 };

    while (true) {
        StretchBlt(dcCopy, rand() % 10, rand() % 10, w, h, dc, rand() % -10, rand() % -10, w, h, SRCCOPY);
        AlphaBlend(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        if (rand() % 10 == 0) {
            Sleep(5);
            InvalidateRect(NULL, NULL, TRUE);
        }
    }
    return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
