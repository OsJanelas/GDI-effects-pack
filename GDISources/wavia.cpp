#include <windows.h>
#include <cstdlib>
#include <ctime>

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	HDC desk;
	int x, t, sw, sh;
	while (true) {
		desk = GetDC(0);
		sw = GetSystemMetrics(SM_CXSCREEN);
		sh = GetSystemMetrics(SM_CYSCREEN);
		t = (rand() % sh);
		x = (rand() % 5);
		if (x == 0) {
			StretchBlt(desk, 2, t, sw + 4, t, desk, 0, t, sw, t, SRCCOPY);
		}
		else if (x == 1) {
			StretchBlt(desk, 0, t, sw, t, desk, 2, t, sw + 4, t, SRCCOPY);
		};
	};
}