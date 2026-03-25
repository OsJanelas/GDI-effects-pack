#include <iostream>
#include <windows.h>
#include <cmath>

DWORD random() {
	DWORD number = __rdtsc();

	number ^= number << 13;
	number ^= number << 17;
	number ^= number << 5;

	return number;
}

int main()
{
	HDC hdc = GetDC(0);
	RECT rekt;
	POINT rotate[3];

	while (true) {
		int rnd = random() % 65 + 60;

		for (int i = 0; i < rnd; i += 20) {
			GetWindowRect(GetDesktopWindow(), &rekt);

			rotate[0].x = rekt.left + i;
			rotate[0].y = rekt.top - i;
			rotate[1].x = rekt.right + i;
			rotate[1].y = rekt.top + i;
			rotate[2].x = rekt.left - i;
			rotate[2].y = rekt.bottom - i;

			PlgBlt(hdc, rotate, hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
			Sleep(40);
		}
		for (int i = 0; i < rnd; i += 20) {
			GetWindowRect(GetDesktopWindow(), &rekt);

			rotate[0].x = rekt.left - i;
			rotate[0].y = rekt.top + i;
			rotate[1].x = rekt.right - i;
			rotate[1].y = rekt.top - i;
			rotate[2].x = rekt.left + i;
			rotate[2].y = rekt.bottom + i;

			PlgBlt(hdc, rotate, hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
			Sleep(40);
		}
	}
}