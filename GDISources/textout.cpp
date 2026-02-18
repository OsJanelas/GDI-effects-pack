#include <windows.h>
#include <iostream>
#include <vector>

// Linkando as bibliotecas necessárias
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

int main() {
    // Esconde a janela do console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);

    // Frases do seu código
    const char* lpStrings[] = {
        "Do pills work?", "I don't want live anymore", "Waiting for my end",
        "Everyday is a infinite loop", "My thoughts are killing me", "Kill me",
        "Dissociating", "Fleeing reality", "Am I Real?", "I'am not safe",
        "HELP ME", "cold.", "Transcending", "Anxiety", "Hear the voices",
        "Trapped by illusion", "Full of emptiness", "Dreaming forever",
        "Questioning my existence", "Losing control regressively", "pain",
        "Don't want to suffer anymore", "Panic attacks", "Dark forever",
        "Faking my happiness", "Seeking eternal peace", "Can't breathe", "sad",
        "Existential Crisis", "Viewing shadows", "Beyond the stars", "Alone",
        "Can't kill myself", "What am I", "Made with rotten flesh", "Isolated",
        "Wearing masks", "My final destination", "Hurting myself", "Farewell"
    };

    int stringCount = sizeof(lpStrings) / sizeof(lpStrings[0]);

    while (true) {
        HDC hdc = GetDC(NULL);

        // Configuração da Fonte (baseado no seu LOGFONT)
        LOGFONT lFont = { 0 };
        lFont.lfWidth = 20;
        lFont.lfHeight = 50;
        lFont.lfOrientation = rand() % 3600; // Rotação aleatória para caos visual
        lFont.lfEscapement = lFont.lfOrientation;
        lFont.lfWeight = 800;
        lFont.lfUnderline = TRUE;
        lFont.lfQuality = DRAFT_QUALITY;
        lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
        lstrcpy(lFont.lfFaceName, L"Arial");

        HFONT hFont = CreateFontIndirect(&lFont);
        SelectObject(hdc, hFont);

        // Define cores aleatórias (estilo "glitch")
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkMode(hdc, TRANSPARENT);

        // Lógica de sorteio (rand() % 25 == 24 como no seu snippet)
        if (rand() % 25 == 24) {
            int x = rand() % w;
            int y = rand() % h;
            const char* selectedText = lpStrings[rand() % stringCount];

            // Desenha o texto na tela
            TextOutA(hdc, x, y, selectedText, strlen(selectedText));
        }

        // Limpeza para não vazar memória
        DeleteObject(hFont);
        ReleaseDC(NULL, hdc);

        // Sleep curto para não travar o processador, mas manter o efeito rápido
        Sleep(10);
    }

    return 0;
}