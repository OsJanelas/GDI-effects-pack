using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace GdiFlashEffect
{
    class Program
    {
        // --- Estruturas e Enums ---

        [StructLayout(LayoutKind.Sequential)]
        public struct RGBQUAD
        {
            public byte rgbBlue;
            public byte rgbGreen;
            public byte rgbRed;
            public byte rgbReserved;
        }

        // Operação Raster para Inversão de Padrão
        public enum TernaryRasterOperations : uint
        {
            PATINVERT = 0x005A0049
        }

        // --- Importações de DLL (P/Invoke) ---

        [DllImport("user32.dll")]
        static extern IntPtr GetDC(IntPtr hWnd);

        [DllImport("user32.dll")]
        static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

        [DllImport("gdi32.dll")]
        static extern IntPtr CreateSolidBrush(uint crColor);

        [DllImport("gdi32.dll")]
        static extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj);

        [DllImport("gdi32.dll")]
        static extern bool PatBlt(IntPtr hdc, int nXLeft, int nYLeft, int nWidth, int nHeight, TernaryRasterOperations dwRop);

        [DllImport("gdi32.dll")]
        static extern bool DeleteObject(IntPtr hObject);

        [DllImport("user32.dll")]
        static extern int GetSystemMetrics(int nIndex);

        const int SM_CXSCREEN = 0;
        const int SM_CYSCREEN = 1;

        // --- Função Principal ---

        public static void Main()
        {
            // Opcional: Esconde a janela do console
            // [DllImport("kernel32.dll")] static extern IntPtr GetConsoleWindow();
            // [DllImport("user32.dll")] static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
            // ShowWindow(GetConsoleWindow(), 0);

            Random r = new Random();

            // Correção das dimensões da tela
            int x = GetSystemMetrics(SM_CXSCREEN);
            int y = GetSystemMetrics(SM_CYSCREEN);

            // Cores em formato COLORREF (0xBBGGRR)
            uint[] rndclr = { 0x0000FF, 0xBC00FF, 0x33FF00, 0x00F7FF, 0xEF0000 };

            while (true)
            {
                IntPtr hdc = GetDC(IntPtr.Zero);

                // Cria um pincel com uma cor aleatória da lista
                uint color = rndclr[r.Next(rndclr.Length)];
                IntPtr brush = CreateSolidBrush(color);

                // Seleciona o pincel no contexto da tela
                IntPtr oldBrush = SelectObject(hdc, brush);

                // Aplica a inversão de padrão (Flash colorido)
                PatBlt(hdc, 0, 0, x, y, TernaryRasterOperations.PATINVERT);

                // Limpeza essencial para não travar o GDI do Windows
                SelectObject(hdc, oldBrush);
                DeleteObject(brush);
                ReleaseDC(IntPtr.Zero, hdc); // ReleaseDC é o correto para GetDC(Zero)

                Thread.Sleep(1000); // Intervalo de 1 segundo
            }
        }
    }
}