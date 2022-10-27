#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <CasualLibrary.hpp>

using namespace std;

inline bool is_process_running(const wchar_t* processName)
{
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
            if (!_wcsicmp(entry.szExeFile, processName))
                exists = true;

    CloseHandle(snapshot);
    return exists;
}

int main()
{
    #pragma region is Minecraft open check
    const wstring name(L"Minecraft.Windows.exe");
    const wchar_t* mc_process_name = name.c_str();

    if (is_process_running(mc_process_name) == false)
    {
        MessageBox(NULL, _T("Minecraft Bedrock Edition isn't open! Please open Minecraft Bedrock Edition and then restart the program!"), _T("Error"), NULL);
        return 0;
    }
    #pragma endregion

    Memory::External memory = Memory::External("Minecraft.Windows.exe", true);

    #pragma region Address stuff
    Address fov_base_addr = memory.getModule("Minecraft.Windows.exe");
    Address fov_addr = memory.getAddress(fov_base_addr + 0x0487B460, { 0x10, 0x170, 0x18 });
    #pragma endregion
}