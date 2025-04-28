#include <windows.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>

// 查找进程ID
DWORD FindProcessId(const std::wstring& processName) {
    PROCESSENTRY32 pe32;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (processName == pe32.szExeFile) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return 0;
}

// 注入DLL到目标进程
void InjectDLL(DWORD pid, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cerr << "无法打开进程！" << std::endl;
        return;
    }

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllPath.length() * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteMemory) {
        std::cerr << "无法分配内存！" << std::endl;
        CloseHandle(hProcess);
        return;
    }

    WriteProcessMemory(hProcess, pRemoteMemory, dllPath.c_str(), dllPath.length() * sizeof(wchar_t), NULL);
    
    FARPROC pLoadLibraryW = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    if (pLoadLibraryW) {
        CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryW, pRemoteMemory, 0, NULL);
    }

    CloseHandle(hProcess);
}

int main() {
    std::wstring processName = L"software.exe";  // 你需要替换成目标进程的实际名称
    std::wstring dllPath = L"ReplyBot.dll";  // DLL的完整路径

    DWORD pid = FindProcessId(processName);
    if (pid == 0) {
        std::cerr << "未找到目标进程！" << std::endl;
        return 1;
    }

    InjectDLL(pid, dllPath);
    std::cout << "DLL注入成功！" << std::endl;
    return 0;
}
