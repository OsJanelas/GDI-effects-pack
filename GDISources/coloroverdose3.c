#include <windows.h>

static ULONGLONG n, r;

int randy() {
    n = r;
    n ^= 0x8ebf635bee3c6d25ULL;
    n ^= (n << 5) | (n >> 26);
    n *= 0xf3e05ca5c43e376bULL;
    r = n;
    return (int)(n & 0x7fffffff);
}

int main() {
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(NULL, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    for (int i = 0;; i = (i + 1) % 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, 4 * h * w, data);

        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - time) > 10)
            byte = (BYTE)(randy() % 0xff);

        for (int i = 0; i < w * h; ++i) {
            if ((i % h) == 0 && (randy() % 70) == 0)
                v = randy() % 24;
            *((BYTE*)data + 4 * i + v) -= 5;
        }

        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);

        DeleteObject(hbm);
        DeleteDC(hdcdc);
        ReleaseDC(NULL, desk);
    }
    return 0;
}