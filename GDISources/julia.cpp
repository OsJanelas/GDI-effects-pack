/*
* ORIGINAL CODE BY ARTICZERA
* OPTIMIZED BY OSJANELAS
*/

#include <windows.h>
#include <iostream>

HDC dc = GetDC(NULL);
HDC dcCopy = CreateCompatibleDC(dc);
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main()
{
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

	INT i = 0;

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, w, h, dc, 0, 0, w, h, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;

				double fractalX = (2.5f / w);
				double fractalY = (1.90f / h);

				double cx = x * fractalX - 2.f;
				double cy = y * fractalY - 0.95f;

				double zx = 0;
				double zy = 0;

				int fx = 0;

				while (((zx * zx) + (zy * zy)) < 20 && fx < 20)
				{
					double fczx = zx * zx - zy * zy + cx;
					double fczy = 2 * zx * zy + cy;

					zx = fczx;
					zy = fczy;
					fx++;

					rgbquad[index].rgbRed += fx;
					rgbquad[index].rgbGreen += fx;
					rgbquad[index].rgbBlue += fx;
				}
			}
		}

		i++;
		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w, h, SRCCOPY);
	}

	return 0x00;
}