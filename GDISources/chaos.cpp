#include <windows.h>

#include <iostream>

#pragma comment(lib, "Msimg32.lib")

HDC dc = GetDC(NULL);
HDC dcCopy = CreateCompatibleDC(dc);
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main() {
	HBITMAP bmp = CreateCompatibleBitmap(dc, w, h);
	SelectObject(dcCopy, bmp);

	while (1)
	{
		int fx = rand() % h;

		if (rand() % 3 == 2)
		{
			StretchBlt(dcCopy, rand() % 3, rand() % 3, w, h, dc, rand() % 3, rand() % 3, w, h, SRCCOPY);
			StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, SRCCOPY);
		}

		if (rand() % 3 == 2)
		{
			if (rand() % 25 == 24) BitBlt(dc, 10, 0, w, h, dc, 0, 10, SRCPAINT);
			if (rand() % 25 == 24) BitBlt(dc, -10, 0, w, h, dc, 0, -10, SRCAND);
			if (rand() % 25 == 24) BitBlt(dc, -10, -10, w, h, dc, 0, 0, SRCPAINT);
			if (rand() % 25 == 24) BitBlt(dc, 10, 10, w, h, dc, 0, 0, SRCAND);
		}

		BitBlt(dc, -10, fx, w, -10, dc, 0, fx, SRCCOPY);
		BitBlt(dc, 10, fx, w, 10, dc, 0, fx, SRCCOPY);

		if (rand() % 25 == 24);
	}

	return 0x00;
}