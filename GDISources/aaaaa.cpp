#include <windows.h>

#include <iostream>

#pragma comment(lib, "Msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main() {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	DWORD rop = SRCCOPY;

	bool statementA = true;
	bool statementB = false;

	int scrollX, scrollY;

	int cw = w / 2;
	int ch = h / 2;

	int limit = 25;

	int i = 0;

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, w, h, dc, w, 0, -cw, h, rop);
		StretchBlt(dc, 0, 0, cw, h, dcCopy, 0, 0, w, h, rop);

		StretchBlt(dcCopy, 0, 0, w, h, dc, 0, 0, w, h, rop);

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				int dx = 10;

				//First scroll
				if (statementA)
				{
					scrollX = (x + dx) % w;
					scrollY = (y + dx) % h;
				}

				//Second scroll
				else if (statementB)
				{
					scrollX = (x + dx) % w;
					scrollY = abs(y - dx) % h;
				}

				int srcIndex = scrollY * w + scrollX;

				rgbquad[index] = rgbquad[srcIndex];
			}
		}

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, rop);

		StretchBlt(dcCopy, 0, 0, w, h, dc, 0, 0, w, ch, rop);
		StretchBlt(dc, 0, ch, w, ch, dcCopy, 0, h, w, -h, rop);

		Sleep(20);
	}

	return 0x00;
}