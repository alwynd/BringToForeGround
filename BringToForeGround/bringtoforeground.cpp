#include "bringtoforeground.h"

using namespace std;

// Enum window callback
BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lparam) 
{
    int length = GetWindowTextLengthA(hWnd);
    char buffer[1024];
    char windowText[2048];
    char ImageName[2048];
	
    ZeroMemory(buffer, 1024);
    ZeroMemory(windowText, 2048);
    ZeroMemory(ImageName, 2048);
    DWORD dwProcId = 0;

    // List visible windows with a non-empty title
    long result = GetWindowLongA(hWnd, GWL_STYLE);
    if (IsWindowVisible(hWnd) && length != 0 && (result & (WS_VISIBLE | WS_CAPTION))) 
    {
        DWORD foregroundThreadID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
        
        int wlen = length + 1;
        GetWindowTextA(hWnd, windowText, wlen);
        GetWindowThreadProcessId(hWnd, &dwProcId);
        if (dwProcId)
        {
            HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcId);
            if (hProc)
            {
                HMODULE hMod;
                DWORD cbNeeded;
                bool IsMinimized = IsIconic(hWnd);
                if (EnumProcessModules(hProc, &hMod, sizeof(hMod), &cbNeeded))
                {
                    GetModuleBaseNameA(hProc, hMod, ImageName, MAX_PATH);
                } //if
                CloseHandle(hProc);

                if (_strcmpi(ImageName, "explorer.exe") == 0 && _strcmpi(windowText, "Program Manager") != 0 )
                {
                    sprintf_s(buffer, "brinftoforeground.EnumWindowCallback, Window: hwnd: %p, Title: '%-55s', Executable: '%-55s', Min: '%-5s'\n\0", hWnd, windowText, ImageName, BOOL_TEXT(IsMinimized)); printf(buffer);
                    ShowWindow(hWnd, (IsIconic(hWnd) ? SW_RESTORE : SW_SHOW ));

                    if (foregroundThreadID != dwProcId) {
                        AttachThreadInput(foregroundThreadID, GetCurrentThreadId(), TRUE);                  // Attach this thread's message queue to the message queue of the foreground window
                        keybd_event(VK_MENU, 0, 0, 0);                                                      // Simulate ALT key-down event
                        SetForegroundWindow(hWnd);                                                          // Bring the window to the foreground
                        keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);                                        // Simulate ALT key-up event
                        AttachThreadInput(foregroundThreadID, GetCurrentThreadId(), FALSE);                 // Detach the attached thread
                    } else {
                        // If it's already in the foreground thread, just call SetForegroundWindow
                        SetForegroundWindow(hWnd);
                    }
                }
            } //if
        } //if
		
    }

    return TRUE;
}



//Main entry point.
int main(int argc, char* arg[])
{
    printf("hello"); fflush(stdout);

    EnumWindows(EnumWindowCallback, NULL);
    
    return 0;
}