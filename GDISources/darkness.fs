open System
open System.Runtime.InteropServices
open System.Threading

[<DllImport("user32.dll")>]
extern int GetSystemMetrics(int nIndex)

[<DllImport("user32.dll")>]
extern IntPtr GetDC(IntPtr hWnd)

[<DllImport("user32.dll")>]
extern bool ReleaseDC(IntPtr hWnd, IntPtr hDC)

[<DllImport("gdi32.dll")>]
extern bool BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, uint32 dwRop)

[<DllImport("kernel32.dll")>]
extern void Sleep(uint32 dwMilliseconds)

[<Literal>]
let SRCAND = 0x008800C6u

[<EntryPoint>]
let main _ =
    let w = GetSystemMetrics(0)
    let h = GetSystemMetrics(1)
    let rand = Random()
    while true do
        let hdc = GetDC(IntPtr.Zero)
        BitBlt(hdc, rand.Next(2), rand.Next(12), w, h, hdc, rand.Next(2), rand.Next(12), SRCAND) |> ignore
        Sleep(10u)
        ReleaseDC(IntPtr.Zero, hdc) |> ignore
    0