#include <iostream>
#include <fstream>
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

inline bool check_window_focus()
{
    // if MinecraftZoom is focused
    if (GetConsoleWindow() == GetForegroundWindow())
    {
        return true;
    }
    return false;
}

int main()
{
    char hotkey;

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

    cout << "Input the hotkey you would like to use for zoom: ";

    cin >> hotkey;

    hotkey = toupper(hotkey);

    if (!isalpha(hotkey)) // check if user inputted hotkey is apart of the alphabet
    {
        cout << "Hotkey must be apart of the English Alphabet!" << endl;
        return 1;
    }

    cout << "\nThe hotkey you chose is: " << hotkey << endl;

    cout << "\nRunning Minecraft Zoom..." << endl;

    cout << "\nPress the ESCAPE key to exit...";

    float original_fov = memory.read<float>(fov_addr);

    while (true)
    {
        Sleep(5);

        if (check_window_focus())
        {
            if (GetAsyncKeyState(VK_ESCAPE))
            {
                break;
            }
        }

        if (GetAsyncKeyState(hotkey) & 0x8000)
        {
            memory.write<float>(fov_addr, 30, true);
        }
        else
        {
            memory.write<float>(fov_addr, original_fov, true);
        }
    }
}