using System;
using System.Runtime.InteropServices;
using System.Threading;

class Program
{
    const int SM_CXSCREEN = 0;
    const int SM_CYSCREEN = 1;
    const int NOTSRCCOPY = 0x00330008;

    [DllImport("user32.dll")]
    static extern IntPtr GetDC(IntPtr hWnd);

    [DllImport("user32.dll")]
    static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

    [DllImport("gdi32.dll")]
    static extern bool BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                              IntPtr hdcSrc, int nXSrc, int nYSrc, int dwRop);

    [DllImport("user32.dll")]
    static extern int GetSystemMetrics(int nIndex);

    static void Main()
    {
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        Random rand = new Random();

        while (true)
        {
            IntPtr hdc = GetDC(IntPtr.Zero);
            int x = rand.Next(w);
            BitBlt(hdc, x, 10, 1, h, hdc, x, 0, NOTSRCCOPY);
            Thread.Sleep(2);
            ReleaseDC(IntPtr.Zero, hdc);
        }
    }
}