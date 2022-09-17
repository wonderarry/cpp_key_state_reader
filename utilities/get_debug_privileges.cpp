#pragma once
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

void get_debug_privileges(){
    HANDLE handle_token;
    LUID luid;
    TOKEN_PRIVILEGES tkp;

    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &handle_token);
    
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(handle_token, false, &tkp, sizeof(tkp), NULL, NULL);
    CloseHandle(handle_token);

}
