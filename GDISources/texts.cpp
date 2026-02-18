// This code is made by ArTicZera/JhoPro

#include <windows.h>

#include <iostream>

#pragma comment(lib, "Msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main() {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	LOGFONTW lFont = { 0 };

	//Setup our font style
	lFont.lfWidth = 20;
	lFont.lfHeight = 50;
	lFont.lfOrientation = 400;
	lFont.lfWeight = 800;
	lFont.lfUnderline = TRUE;
	lFont.lfQuality = DRAFT_QUALITY;
	lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;

	lstrcpy(lFont.lfFaceName, L"Arial Black");

	while (true)
	{
		if (rand() % 25 == 24)
		{
			LPCSTR lpStrings[] =
			{
				"Your text here"
			};

			//Setup the angle
			lFont.lfEscapement = rand() % 60;

			HFONT hFont = CreateFontIndirectW(&lFont);
			SelectObject(dc, hFont);

			//Setup the colors
			SetTextColor(dc, RGB(rand() % 0xFF, rand() % 0xFF, rand() % 0xFF));
			SetBkColor(dc, RGB(rand() % 0xA0, rand() % 0xA0, rand() % 0xA0));

			int index = rand() % 40;

			TextOutA(dc, rand() % w, rand() % h, lpStrings[index], lstrlenA(lpStrings[index]));

			if (rand() % 25 == 24)
			{
				if (rand() % 2 == 0)
				{
					BitBlt(dc, 1, 0, w, h, dc, 0, 1, 0x999999);
				}
				else
				{
					BitBlt(dc, 1, 0, w, h, dc, 0, 1, 0x666666);
				}
			}

			Sleep(rand() % 5);
		}
	}

	return 0x00;
}