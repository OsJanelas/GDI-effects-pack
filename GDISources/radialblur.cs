using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace GdiPayload
{
    class Program
    {
        // Importando métricas do sistema manualmente
        [DllImport("user32.dll")]
        static extern int GetSystemMetrics(int nIndex);

        // Constantes para as métricas
        const int SM_CXSCREEN = 0;
        const int SM_CYSCREEN = 1;

        [StructLayout(LayoutKind.Sequential)]
        public struct POINT { public int X; public int Y; }

        [StructLayout(LayoutKind.Sequential)]
        public struct BLENDFUNCTION
        {
            public byte BlendOp; public byte BlendFlags;
            public byte SourceConstantAlpha; public byte AlphaFormat;
            public BLENDFUNCTION(byte alpha) { BlendOp = 0; BlendFlags = 0; SourceConstantAlpha = alpha; AlphaFormat = 0; }
        }

        [DllImport("user32.dll")] static extern IntPtr GetDC(IntPtr hWnd);
        [DllImport("user32.dll")] static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);
        [DllImport("gdi32.dll")] static extern IntPtr CreateCompatibleDC(IntPtr hdc);
        [DllImport("gdi32.dll")] static extern IntPtr CreateCompatibleBitmap(IntPtr hdc, int nWidth, int nHeight);
        [DllImport("gdi32.dll")] static extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj);
        [DllImport("gdi32.dll")] static extern bool DeleteObject(IntPtr hObject);
        [DllImport("gdi32.dll")] static extern bool DeleteDC(IntPtr hdc);
        [DllImport("gdi32.dll")] static extern bool PlgBlt(IntPtr hdcDest, POINT[] lpPoint, IntPtr hdcSrc, int nXSrc, int nYSrc, int nWidth, int nHeight, IntPtr hbmMask, int xMask, int yMask);
        [DllImport("msimg32.dll")] static extern bool AlphaBlend(IntPtr hDest, int x, int y, int w, int h, IntPtr hSrc, int sx, int sy, int sw, int sh, BLENDFUNCTION f);

        static void Main()
        {
            // Pegando a largura e altura via API Nativa, sem depender de Screen.PrimaryScreen
            int w = GetSystemMetrics(SM_CXSCREEN);
            int h = GetSystemMetrics(SM_CYSCREEN);

            Random r = new Random();
            POINT[] lppoint = new POINT[3];

            while (true)
            {
                IntPtr hdc = GetDC(IntPtr.Zero);
                IntPtr mhdc = CreateCompatibleDC(hdc);
                IntPtr hbit = CreateCompatibleBitmap(hdc, w, h);
                IntPtr holdbit = SelectObject(mhdc, hbit);

                // Efeito de inclinação/paralelogramo
                if (r.Next(2) == 1)
                {
                    lppoint[0] = new POINT { X = 0 + 30, Y = 0 - 30 };
                    lppoint[1] = new POINT { X = w + 30, Y = 0 + 30 };
                    lppoint[2] = new POINT { X = 0 - 30, Y = h - 30 };
                }
                else
                {
                    lppoint[0] = new POINT { X = 0 - 30, Y = 0 + 30 };
                    lppoint[1] = new POINT { X = w - 30, Y = 0 - 30 };
                    lppoint[2] = new POINT { X = 0 + 30, Y = h + 30 };
                }



                // Realiza a distorção
                PlgBlt(mhdc, lppoint, hdc, 0, 0, w, h, IntPtr.Zero, 0, 0);
                // Aplica de volta com transparência (Alpha de 60/255)
                AlphaBlend(hdc, 0, 0, w, h, mhdc, 0, 0, w, h, new BLENDFUNCTION(60));

                // Limpeza essencial de memória
                SelectObject(mhdc, holdbit);
                DeleteObject(hbit);
                DeleteDC(mhdc);
                ReleaseDC(IntPtr.Zero, hdc);

                Thread.Sleep(50);
            }
        }
    }
}