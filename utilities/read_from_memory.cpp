#pragma once
#include <windows.h>
#include <cstdio>

int read_int(HANDLE process_handle, int addr)
{
    int buffer = 0;
    SIZE_T NumberOfBytesToRead = sizeof(buffer); //this is equal to 4
    SIZE_T NumberOfBytesActuallyRead;
    BOOL success = ReadProcessMemory(process_handle, (LPCVOID)addr, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
    if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead) {
        fprintf(stderr, "Failed to read an integer: %d\n", GetLastError());
        return -1;
    }
    return buffer;
}

int read_bool(HANDLE process_handle, int addr){
    bool buffer = 0;
    SIZE_T number_of_bytes_to_read = sizeof(buffer);
    SIZE_T number_of_bytes_actuall_read;
    ReadProcessMemory(process_handle, (LPCVOID) addr, &buffer, number_of_bytes_to_read, &number_of_bytes_actuall_read);
    return buffer;
}