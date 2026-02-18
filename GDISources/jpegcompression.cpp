#include <windows.h>

#include <iostream>

#pragma comment(lib, "Msimg32.lib")

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main() {
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 8;
	int hs = h / 8;

	BLENDFUNCTION blur = { 0 };
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

	//SET OUT BLUR
	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 20;

	INT i = 0;

	while (1)
	{
		StretchBlt(dcCopy, rand() % 1, rand() % 1, ws, hs, dc, rand() % -1, rand() % -1, w, h, SRCCOPY);

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				rgbquad[index].rgbRed += i;
				rgbquad[index].rgbGreen += i;
				rgbquad[index].rgbBlue += i;
			}
		}

		i++;

		Sleep(rand() % 500);
		AlphaBlend(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, blur);
	}

	return 0x00;

}
