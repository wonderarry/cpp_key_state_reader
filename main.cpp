#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <psapi.h>
#pragma comment(lib, "psapi")

#include "headers/utilities.hpp"
#include "headers/models.hpp"

int main()
{
    std::string process_name = "osu!.exe";
    unsigned char ruleset_base_signature[] = {0xC7,0x86,0x48,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0xA1};
    //unsigned char ruleset_base_signature[] = {0xC7,0x86,0x48};
    auto user_space_limit = 0x7FFFFFFF0000;
    get_debug_privileges();
    std::cout << "success\n";
    handle_package osu_handle_package = get_process_handle_by_name(process_name);

    std::cout << "\n----------\nProcess id: "
              << osu_handle_package.process_id;

    std::cout << "\nProcess handle: "
              << osu_handle_package.handle << "\n-------------\n";

    auto query_result = pattern_scan_all(osu_handle_package.handle, ruleset_base_signature, sizeof(ruleset_base_signature));
    printf("\nquery result: %p", query_result);
    int tmp_addr;
    int k1_addr, k2_addr;
    auto prochd = osu_handle_package.handle;

    query_result += 0xB;
    tmp_addr = read_int(prochd, (int) query_result) + 0x4;
    tmp_addr = read_int(prochd, tmp_addr) + 0xA8;
    tmp_addr = read_int(prochd, tmp_addr) + 0x10;
    tmp_addr = read_int(prochd, tmp_addr) + 0x4;
    tmp_addr = read_int(prochd, tmp_addr);
    k1_addr = ((read_int(prochd, tmp_addr + 0x8)) + 0x1C);
    k2_addr = ((read_int(prochd, tmp_addr + 0xC)) + 0x1C);
    
    printf("\n%p %p", k1_addr, k2_addr);
    printf("\n%d", read_bool(prochd,k1_addr));
    //printf("\nObrained data: %f", *query_result);
    // if (query_result == 0){
    //     std::cout << "\nunsuccessful search.";
    // }
    // else{
    //     std::cout << "\nQuery result: " << *(query_result);
    // }
    

    return 0;
}
