#include <windows.h>
#include <vector>

struct WindowState {
    HWND hwnd;
    int originalX, originalY;
    float offset;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0) {
        std::vector<HWND>* windows = (std::vector<HWND>*)lParam;
        windows->push_back(hwnd);
    }
    return TRUE;
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    std::vector<HWND> windowHandles;
    float timer = 0;

    while (true) {
        windowHandles.clear();
        EnumWindows(EnumWindowsProc, (LPARAM)&windowHandles);

        for (HWND hwnd : windowHandles) {
            RECT rect;
            GetWindowRect(hwnd, &rect);

            int jump = (int)(sin(timer + (UINT_PTR)hwnd) * 10);

            SetWindowPos(hwnd, NULL, rect.left, rect.top + jump, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        }

        timer += 0.5f;
        Sleep(30);
    }

    return 0;
}