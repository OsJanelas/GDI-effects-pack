#include <iostream>
#include <windows.h>

#pragma comment(lib, "Msimg32.lib")

HDC dc = GetDC(NULL);
HDC dcCopy = CreateCompatibleDC(dc);
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int main()
{
	{
		while (1)
		{
			for (int x = 0; x <= w; x++)
			{
				for (int y = 0; y <= h; y++)
				{
					int fx = x ^ y;
					COLORREF color = RGB(fx, fx, fx);
					HBRUSH brush = CreateSolidBrush(color);
					SelectObject(dc, brush);

					PatBlt(dc, 0, 0, w, h, PATINVERT);
					Sleep(rand() % 50);
				}
			}
		}

		return 0x00;
	}
}