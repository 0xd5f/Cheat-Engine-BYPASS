#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <tlhelp32.h>

#include <random>

const char* targetProcessName = "Fx9PqzV42XkjamQ7CYvMnB-x86_64.exe";

std::string RandomString(int length) {
    static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string result;
    result.resize(length);

    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];

    return result;
}
DWORD WINAPI RandomTitle(LPVOID)
{
    while (1)
    {
        SetConsoleTitleA(RandomString(16).c_str());
        Sleep(5);
    }
}
DWORD GetProcessID(const char* processName) {
    DWORD processID = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(pe);
        if (Process32FirstW(snapshot, &pe)) {
            do {
                char exeName[MAX_PATH];
                WideCharToMultiByte(CP_ACP, 0, pe.szExeFile, -1, exeName, MAX_PATH, nullptr, nullptr);

                if (_stricmp(exeName, processName) == 0) {
                    processID = pe.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &pe));
        }
        CloseHandle(snapshot);
    }
    return processID;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD windowPID;
    GetWindowThreadProcessId(hwnd, &windowPID);

    if (windowPID == lParam && IsWindowVisible(hwnd)) {
        std::string newTitle = RandomString(16);
        SetWindowTextA(hwnd, newTitle.c_str());
        std::cout << "[+] The window is renamed to: " << newTitle << std::endl;
    }
    return TRUE;
}

DWORD WINAPI RenameProcessWindows(LPVOID) {
    while (true) {
        DWORD processID = GetProcessID(targetProcessName);
        if (processID != 0) {
            EnumWindows(EnumWindowsProc, processID);
        }
        Sleep(1);
    }
}
void LaunchCheatEngine() {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessA("Fx9PqzV42XkjamQ7CYvMnB-x86_64.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cout << "Launched Cheat Engine, PID: " << pi.dwProcessId << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "Launch error Cheat Engine!" << std::endl;
    }
}
int main() {
    std::cerr << "CERBERUS SOFTWARE\n" << std::endl;
    LaunchCheatEngine();
    srand(GetTickCount());

    CreateThread(0, 0, RandomTitle, 0, 0, 0);
    CreateThread(nullptr, 0, RenameProcessWindows, nullptr, 0, nullptr);

    std::cin.get();
    return 0;
}

