using System;
using System.Runtime.InteropServices;
using System.Threading;

class Program
{
    [DllImport("user32.dll")]
    static extern int GetSystemMetrics(int nIndex);

    [DllImport("user32.dll")]
    static extern IntPtr GetDC(IntPtr hWnd);

    [DllImport("gdi32.dll")]
    static extern bool BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                              IntPtr hdcSrc, int nXSrc, int nYSrc, uint dwRop);

    [DllImport("user32.dll")]
    static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

    const uint SRCCOPY = 0x00CC0020;

    static void Main()
    {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);

        while (true)
        {
            IntPtr hdc = GetDC(IntPtr.Zero);
            BitBlt(hdc, -30, 0, w, h, hdc, 0, 0, SRCCOPY);
            BitBlt(hdc, w - 30, 0, w, h, hdc, 0, 0, SRCCOPY);
            ReleaseDC(IntPtr.Zero, hdc);
            Thread.Sleep(10);
        }
    }
}