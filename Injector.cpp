#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

//**//
std::wstring processName = L"example.exe"; // Replace with the actual process name
std::string dllName = "example.dll"; // Replace with the actual DLL name
//**//

DWORD GetProcessIdByName(const std::wstring& processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(snapshot, &processEntry)) {
        CloseHandle(snapshot);
        return 0;
    }

    do {
        std::wstring entryName(processEntry.szExeFile);
        if (entryName == processName) {
            CloseHandle(snapshot);
            return processEntry.th32ProcessID;
        }
    } while (Process32NextW(snapshot, &processEntry));

    CloseHandle(snapshot);
    return 0;
}


std::string GetExecutablePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path(buffer);
    size_t pos = path.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : path.substr(0, pos);
}

int main()
{
    std::string exePath = GetExecutablePath();

    std::string dllPath = exePath + "\\" + dllName;
    std::cout << dllPath << std::endl;
    DWORD pid = GetProcessIdByName(processName);
    {
        char DLL[260];
        dllPath.copy(DLL, dllPath.size());
        DLL[dllPath.size()] = '\0';
        HANDLE HMODULE = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
        LPVOID AllocAddress = VirtualAllocEx(HMODULE, nullptr, 260, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(HMODULE, AllocAddress, DLL, 260, nullptr);
        CreateRemoteThread(HMODULE, nullptr, 0, LPTHREAD_START_ROUTINE(LoadLibrary), AllocAddress, 0, nullptr);
    }
    return 0;
}
