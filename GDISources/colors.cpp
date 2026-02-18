#include <iostream>
#include <windows.h>

#pragma comment(lib, "Msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main()
{
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 16;
	int hs = h / 16;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;

	while (1)
	{
		HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(dcCopy, hBrush);
		SelectObject(dc, hBrush);

		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCPAINT | PATINVERT);

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int i = rand() % 10;

				rgbquad[index].rgbRed += i;
				rgbquad[index].rgbGreen += i;
				rgbquad[index].rgbBlue += i;
			}
		}

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCPAINT | PATINVERT);

		Sleep(rand() % 10);
	}

	return 0x00;
}