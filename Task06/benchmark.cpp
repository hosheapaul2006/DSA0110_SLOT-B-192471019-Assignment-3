#include "text.hpp"
#include "../tools/checks.hpp"
#include <chrono>
#include <cstdint>
#include <string>
int main(int argc,char** argv) {
    Tests tests(argc,argv);constexpr int operations=100000;
    const std::string sa(256,'A'),sb(128,'B'),sc(64,'C');
    const std::string expected=sa+sb+sc;
    const Text a(sa.c_str()),b(sb.c_str()),c(sc.c_str());
    const std::uint64_t expectedChecksum=static_cast<std::uint64_t>(operations)*(expected.size()+'A'+'C');
    std::cout<<"trial,design,operations,milliseconds,checksum,custom_allocation_count\n";
    for(int trial=1;trial<=5;++trial) {
        auto direct=[&]() {
            const auto allocations=Text::allocations;std::uint64_t checksum=0;
            const auto begin=std::chrono::steady_clock::now();
            for(int i=0;i<operations;++i) { Text result=a+b+c;checksum+=result.size()+static_cast<unsigned char>(result.c_str()[0])+static_cast<unsigned char>(result.c_str()[result.size()-1]); }
            const auto end=std::chrono::steady_clock::now();const auto measuredAllocations=Text::allocations-allocations;
            tests.check(checksum==expectedChecksum && std::string((a+b+c).c_str())==expected,"custom direct concatenation output");
            std::cout<<trial<<",custom_direct,"<<operations<<','<<std::chrono::duration<double,std::milli>(end-begin).count()<<','<<checksum<<','<<measuredAllocations<<'\n';
        };
        auto standard=[&]() {
            std::uint64_t checksum=0;const auto begin=std::chrono::steady_clock::now();
            for(int i=0;i<operations;++i) { std::string result=sa+sb+sc;checksum+=result.size()+static_cast<unsigned char>(result.front())+static_cast<unsigned char>(result.back()); }
            const auto end=std::chrono::steady_clock::now();
            tests.check(checksum==expectedChecksum && sa+sb+sc==expected,"std string concatenation output");
            std::cout<<trial<<",std_string,"<<operations<<','<<std::chrono::duration<double,std::milli>(end-begin).count()<<','<<checksum<<",not_instrumented\n";
        };
        auto alternative=[&]() {
            const auto allocations=Text::allocations;std::uint64_t checksum=0;
            const auto begin=std::chrono::steady_clock::now();
            for(int i=0;i<operations;++i) { Text result=plusOnCopy(plusOnCopy(a,b),c);checksum+=result.size()+static_cast<unsigned char>(result.c_str()[0])+static_cast<unsigned char>(result.c_str()[result.size()-1]); }
            const auto end=std::chrono::steady_clock::now();const auto measuredAllocations=Text::allocations-allocations;
            tests.check(checksum==expectedChecksum && std::string(plusOnCopy(plusOnCopy(a,b),c).c_str())==expected,"copy plus equals concatenation output");
            std::cout<<trial<<",custom_copy_plus,"<<operations<<','<<std::chrono::duration<double,std::milli>(end-begin).count()<<','<<checksum<<','<<measuredAllocations<<'\n';
        };
        if(trial%2) { direct();standard();alternative(); } else { alternative();standard();direct(); }
    }
}
