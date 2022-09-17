#pragma once
#include "../headers/models.hpp"
#include <iostream>

handle_package get_process_handle_by_name(std::string given_name)
{
    std::wstring process_name = std::wstring(given_name.begin(), given_name.end());
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    std::cout << "\ninstart";
    if (Process32FirstW(snapshot, &entry) == TRUE)
    {
        std::cout << "\nfirsttrue";
        while (Process32NextW(snapshot, &entry) == TRUE)
        {
            std::cout << "\niterating, result = " << wcscmp(entry.szExeFile, (process_name.c_str()));
            if (wcscmp(entry.szExeFile, (process_name.c_str())) == 0)
            {
                HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                handle_package returned_package{process_handle, entry.th32ProcessID};
                return returned_package;
            }
        }
    }
    return {nullptr, 0};
}
