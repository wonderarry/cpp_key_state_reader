#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <typeinfo>

#include "../headers/models.hpp"

MEMORY_BASIC_INFORMATION virtual_query(HANDLE process_handle, unsigned char *region_offset)
{
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQueryEx(process_handle, region_offset, &mbi, sizeof(mbi));
    return mbi;
}

void *memmem(void *haystack, size_t haystacklen, void *needle, size_t needlelen)
{
    char *haystack_tracker = (char*)haystack;
    while (haystacklen >= needlelen + (haystack_tracker - (char*)haystack)){
        if (memcmp(haystack_tracker, (char*)needle, needlelen) == 0){ //match
            return haystack_tracker;
        }
        haystack_tracker += 1;
    }
    return nullptr;
}
char *read_bytes(HANDLE process_handle, unsigned char *region_offset, size_t buffer_size)
{
    printf("\nHandle: %p\nOffset in read_bytes: %p\nBuffer_size: %ld\n", process_handle, region_offset, buffer_size);

    char *buffer = (char *)(malloc(buffer_size));
    SIZE_T result_len = 0;

    ReadProcessMemory(process_handle, (LPCVOID)region_offset, (LPVOID)buffer, buffer_size, &result_len);
    return buffer;
}

int search_pattern_in_bytes(char *buffer, int buffer_length, unsigned char pattern[], int pattern_length)
{
    // for (int i = 0; i < pattern_length; ++i){
    //     printf("\npattern item %d: %x", i, pattern[i]);
    // }
    // for (int i = 0; i < buffer_length; ++i){
    //     printf("\nbuffer item %d: %x", i, buffer[i]);
    // }
    void *res = memmem((void*)buffer, (size_t)buffer_length, (void*)pattern, (size_t)pattern_length);
    if (res == nullptr){
        return -1;
    }
    return int((char*)res - buffer);

}

page_search_result scan_pattern_page(HANDLE process_handle, unsigned char **region_offset, unsigned char pattern[], int pattern_length)
{
    page_search_result evaluated_result;
    MEMORY_BASIC_INFORMATION mbi = virtual_query(process_handle, *region_offset);

    *region_offset = (unsigned char *)mbi.BaseAddress + mbi.RegionSize;
    // printf("%p", ptr);
    // std::cout << "\nregion offset: " << region_offset;
    printf("\nRegion offset: %d", *region_offset);
    printf("\nBase address: %d", mbi.BaseAddress);
    printf("\nRegion size: %d", mbi.RegionSize);
    evaluated_result.next_region = *region_offset;

    auto x = (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READWRITE | PAGE_READONLY);
    if (!(mbi.Protect & x) || mbi.State != MEM_COMMIT)
    {
        printf("\nmbi.Protect: %d; mbi.State: %d", mbi.Protect, mbi.State);
        evaluated_result.page_found = 0;

        return evaluated_result;
    }
    //std::cin.get();
    auto page_bytes = read_bytes(process_handle, *region_offset, mbi.RegionSize);
    printf("\npage_bytes: %p", page_bytes);
    //std::cin.get();
    int match_index = search_pattern_in_bytes(page_bytes, mbi.RegionSize, pattern, pattern_length);
    printf("\nMatch index: %d", match_index);
    //std::cin.get();
    if (match_index == -1)
    {   
        evaluated_result.page_found = 0;
    }
    else
    {
        printf("\nGood page search %d", match_index);
        evaluated_result.page_found = *region_offset + match_index;
    }
    printf("\nFound the following address %p", evaluated_result.page_found);
    return evaluated_result;
}

unsigned char *pattern_scan_all(HANDLE process_handle, unsigned char pattern[], int pattern_length)
{

    unsigned char *next_region = NULL;
    auto user_space_limit = 0x7FFFFFFF0000;
    //for (int i = 0; i < 100000; ++i)
    while ((long long)next_region < user_space_limit)
    {
        std::cout << "\n----------\nIterating in the loop...\n";
        printf("\nBefore everything - next region = %p", next_region);
        page_search_result search_res = scan_pattern_page(
            process_handle,
            &next_region,
            pattern,
            pattern_length);
        if (search_res.page_found)
        {   
            return search_res.page_found;
        }
    }
    return 0;
}

// std::cout << "\n";