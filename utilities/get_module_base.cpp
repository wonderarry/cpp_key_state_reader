#pragma once
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib, "psapi")

int get_module_base(HANDLE process_handle, std::string module_name)
{
    std::wstring wstr_module_name = std::wstring(module_name.begin(), module_name.end());

    HMODULE *handle_modules = NULL;
    wchar_t sz_buffer[50];
    DWORD modules_counter;
    DWORD base_address = -1;

    EnumProcessModules(process_handle, handle_modules, 0, &modules_counter);
    handle_modules = new HMODULE[modules_counter / sizeof(HMODULE)];

    if (EnumProcessModules(process_handle, handle_modules, modules_counter, &modules_counter))
    {
        for (size_t i = 0; i < modules_counter / sizeof(HMODULE); i++)
        {

            if (GetModuleBaseNameW(process_handle, handle_modules[i], sz_buffer, sizeof(sz_buffer)))
            {
                if (wstr_module_name.compare(sz_buffer) == 0)
                {
                    base_address = (DWORD)handle_modules[i];
                    break;
                }
            }
        }
    }

    delete[] handle_modules;
    return base_address;
}