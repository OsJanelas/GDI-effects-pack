#include <windows.h>

int main() {
	while (1) {
		HDC hdc = GetDC(0);
		int x = SM_CXSCREEN;
		int y = SM_CYSCREEN;
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(0);
		BitBlt(hdc, rand() % 2, rand() % 10, w, h, hdc, rand() % 2, rand() % 10, SRCPAINT);
		Sleep(10);
	}
}