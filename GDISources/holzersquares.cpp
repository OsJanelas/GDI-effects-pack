#include <windows.h>
#include <cmath>
#include <iostream>

DWORD random() {
	DWORD number = __rdtsc();

	number ^= number << 13;
	number ^= number << 17;
	number ^= number << 5;

	return number;
}


int main()
{
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	HDC hdc = GetDC(0);

	int ys = 0;

	while (true) {
		HBRUSH brush = CreateSolidBrush(RGB(random() % 255, random() % 255, random() % 255));
		SelectObject(hdc, brush);

		for (int i = 0; i < x; i += 100) {
			Rectangle(hdc, i, ys + 100, 100 + i, ys);
			Sleep(10);
		}

		ys += 100;
		if (ys >= y) {
			ys = 0;
		}

		Sleep(30);
	}
}