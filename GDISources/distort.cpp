typedef struct IUnknown IUnknown;

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")

#pragma warning(disable : 4996)

#include <Windows.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

int waveDelay = rand() % 30;

typedef struct
{
	float x;
	float y;
	float z;
} VERTEX;

typedef struct
{
	int vtx0;
	int vtx1;
} EDGE;

namespace Math
{
	FLOAT SineWave(FLOAT a, FLOAT b, FLOAT c, FLOAT d)
	{
		return a * sin(2 * PI * b * c / d);
	}
}

int main()
{
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	/*
	  Intensity:
	  3 - WEAK
	  5 - HARD
	*/
	float warpIntensity = 3;

	while (1)
	{
		for (int i = 1; i <= warpIntensity; i++)
		{
			int size = rand() % 400;

			int wsrc = rand() % w;
			int hsrc = rand() % h;

			BitBlt(dc, wsrc + rand() % i, hsrc + rand() % i, size, size, dc, wsrc, hsrc, SRCCOPY);
		}

		Sleep(rand() % 2);

	}

	return 0x00;
}