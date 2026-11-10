#include "auditorium.hpp"
#include "../tools/checks.hpp"
#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <psapi.h>
std::size_t residentBytes() {
    PROCESS_MEMORY_COUNTERS counters{};
    if(!K32GetProcessMemoryInfo(GetCurrentProcess(),&counters,sizeof(counters))) return 0;
    return counters.WorkingSetSize;
}
#else
#include <fstream>
#include <string>
std::size_t residentBytes() {
    std::ifstream input("/proc/self/status");std::string key;
    while(input>>key) { if(key=="VmRSS:") { std::size_t kb=0;input>>kb;return kb*1024; }std::string rest;std::getline(input,rest); }
    return 0;
}
#endif
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    std::cout << "rows,seats,payload_bytes,row_pointer_bytes,allocation_calls,process_resident_bytes\n";
    for(int rows:{10,100,1000}) {
        const int columns=rows*2;
        Auditorium event("Memory",rows,columns,false);
        // Touch all cells so the process measurement includes committed pages.
        for(int row=0;row<rows;++row) for(int seat=0;seat<columns;++seat) event.book(row,seat);
        tests.check(event.payloadBytes()==static_cast<std::size_t>(rows)*columns,"requested seat payload bytes");
        tests.check(event.pointerBytes()==static_cast<std::size_t>(rows)*sizeof(char*) && event.allocations()==static_cast<std::size_t>(rows)+1,"pointer storage and allocation count");
        tests.check(event.booked()==static_cast<std::size_t>(rows)*columns,"all allocated seat cells touched");
        std::cout << rows << ',' << columns << ',' << event.payloadBytes() << ',' << event.pointerBytes()
                  << ',' << event.allocations() << ',' << residentBytes() << '\n';
    }
}
