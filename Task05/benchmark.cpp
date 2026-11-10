#include "playlist.hpp"
#include "../tools/checks.hpp"
#include <array>
#include <chrono>
#include <cstdint>
int main(int argc,char** argv) {
    Tests tests(argc,argv);constexpr int operations=10000000;
    std::array<unsigned,4096> indices{};std::array<unsigned long long,1024> counts{};
    std::uint32_t state=192471019u;
    for(auto& index:indices) { state=state*1664525u+1013904223u;index=state%1024u; }
    for(int i=0;i<operations;++i) ++counts[indices[static_cast<unsigned>(i)&4095u]];
    unsigned long long expectedOld=0;for(auto count:counts) expectedOld+=count*(count-1)/2;
    std::cout<<"trial,design,operations,milliseconds,old_value_checksum,total_seconds\n";
    for(int trial=1;trial<=5;++trial) {
        auto measure=[&](const char* name,auto increment,bool consumed) {
            std::array<Time,1024> timers{};unsigned long long checksum=0;
            const auto start=std::chrono::steady_clock::now();
            for(int i=0;i<operations;++i) checksum+=increment(timers[indices[static_cast<unsigned>(i)&4095u]]);
            const auto end=std::chrono::steady_clock::now();
            bool contents=true;unsigned long long total=0;
            for(unsigned i=0;i<timers.size();++i) { contents=contents && timers[i].seconds()==counts[i];total+=timers[i].seconds(); }
            tests.check(contents && total==operations && checksum==(consumed?expectedOld:0),"prefix postfix benchmark state and returned values");
            std::cout<<trial<<','<<name<<','<<operations<<','<<std::chrono::duration<double,std::milli>(end-start).count()<<','<<checksum<<','<<total<<'\n';
        };
        auto prefix=[](Time& t)->unsigned long long { ++t;return 0; };
        auto postfix=[](Time& t)->unsigned long long { t++;return 0; };
        auto prefixUsed=[](Time& t) { return (++t).seconds()-1; };
        auto postfixUsed=[](Time& t) { return (t++).seconds(); };
        if(trial%2) { measure("prefix_discarded",prefix,false);measure("postfix_discarded",postfix,false); }
        else { measure("postfix_discarded",postfix,false);measure("prefix_discarded",prefix,false); }
        measure("prefix_value_used",prefixUsed,true);measure("postfix_value_used",postfixUsed,true);
    }
}
