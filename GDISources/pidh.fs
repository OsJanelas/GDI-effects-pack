open System
open System.Runtime.InteropServices

[<Struct; StructLayout(LayoutKind.Explicit)>]
type RGBQUAD =
    struct
        [<FieldOffset(0)>]
        val mutable rgb: uint32
        [<FieldOffset(0)>]
        val mutable r: byte
        [<FieldOffset(1)>]
        val mutable g: byte
        [<FieldOffset(2)>]
        val mutable b: byte
        [<FieldOffset(3)>]
        val mutable Reserved: byte
    end

[<Struct>]
type BITMAPINFOHEADER =
    struct
        val mutable biSize: uint32
        val mutable biWidth: int32
        val mutable biHeight: int32
        val mutable biPlanes: uint16
        val mutable biBitCount: uint16
        val mutable biCompression: uint32
        val mutable biSizeImage: uint32
        val mutable biXPelsPerMeter: int32
        val mutable biYPelsPerMeter: int32
        val mutable biClrUsed: uint32
        val mutable biClrImportant: uint32
    end

[<Struct>]
type BITMAPINFO =
    struct
        val mutable bmiHeader: BITMAPINFOHEADER
        // We don't need bmiColors for 32bpp
    end

module Native =
    [<DllImport("user32.dll")>]
    extern IntPtr GetDC(IntPtr hWnd)

    [<DllImport("user32.dll")>]
    extern int ReleaseDC(IntPtr hWnd, IntPtr hDC)

    [<DllImport("gdi32.dll")>]
    extern IntPtr CreateCompatibleDC(IntPtr hdc)

    [<DllImport("gdi32.dll")>]
    extern bool DeleteDC(IntPtr hdc)

    [<DllImport("gdi32.dll")>]
    extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj)

    [<DllImport("gdi32.dll")>]
    extern IntPtr CreateDIBSection(IntPtr hdc, BITMAPINFO& pbmi, uint32 iUsage, nativeint& ppvBits, IntPtr hSection, uint32 dwOffset)

    [<DllImport("gdi32.dll")>]
    extern bool BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
                      IntPtr hdcSrc, int nXSrc, int nYSrc, uint32 dwRop)

    [<DllImport("user32.dll")>]
    extern int GetSystemMetrics(int nIndex)

[<EntryPoint>]
let main argv =
    let SRCCOPY = 0x00CC0020u

    let hdcScreen = Native.GetDC(IntPtr.Zero)
    let hdcMem = Native.CreateCompatibleDC(hdcScreen)

    let w = Native.GetSystemMetrics(0)
    let h = Native.GetSystemMetrics(1)

    let mutable bmi = BITMAPINFO()
    bmi.bmiHeader.biSize <- uint32 (Marshal.SizeOf(typeof<BITMAPINFO>))
    bmi.bmiHeader.biWidth <- w
    bmi.bmiHeader.biHeight <- h
    bmi.bmiHeader.biPlanes <- 1us
    bmi.bmiHeader.biBitCount <- 32us
    bmi.bmiHeader.biCompression <- 0u // BI_RGB

    let mutable ptrBits = nativeint 0
    let hbmTemp = Native.CreateDIBSection(hdcScreen, &bmi, 0u, &ptrBits, IntPtr.Zero, 0u)

    Native.SelectObject(hdcMem, hbmTemp) |> ignore

    // Create a managed span over the pixel buffer
    let pixelCount = w * h
    let pixels = Span<RGBQUAD>(ptrBits.ToPointer(), pixelCount)

    while true do
        let hdcScreen = Native.GetDC(IntPtr.Zero)
        Native.BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY) |> ignore

        for i in 0 .. pixelCount - 1 do
            let x = i % w
            let y = i / w
            let mutable px = pixels.[i]
            px.rgb <- px.rgb + uint32 (x ^^^ y)
            pixels.[i] <- px

        Native.BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY) |> ignore
        Native.ReleaseDC(IntPtr.Zero, hdcScreen) |> ignore
        Native.DeleteDC(hdcScreen) |> ignore

    0