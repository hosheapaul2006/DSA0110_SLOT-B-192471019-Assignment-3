#include "playlist.hpp"
#include "../tools/checks.hpp"
#include <array>
#include <chrono>
#include <cstdint>
int main(int argc,char** argv) {
    Tests tests(argc,argv);constexpr int operations=10000000;
    Playlist playlist;std::array<std::size_t,1024> lengths{};
    for(unsigned i=0;i<1024;++i) { std::string title="Track-"+std::to_string(i);lengths[i]=title.size();playlist.add(title); }
    const Playlist& list=playlist;
    std::array<unsigned,4096> indices{};std::uint32_t state=192471019u;
    for(auto& index:indices) { state=state*1664525u+1013904223u;index=state%1024u; }
    unsigned long long block=0,expected=0;for(auto index:indices) block+=lengths[index];
    expected=block*(operations/4096);for(int i=0;i<operations%4096;++i) expected+=lengths[indices[static_cast<unsigned>(i)]];
    std::cout<<"trial,design,operations,milliseconds,length_checksum\n";
    for(int trial=1;trial<=5;++trial) {
        auto measure=[&](const char* name,auto access) {
            unsigned long long checksum=0;const auto start=std::chrono::steady_clock::now();
            for(int i=0;i<operations;++i) checksum+=access(indices[static_cast<unsigned>(i)&4095u]).size();
            const auto end=std::chrono::steady_clock::now();
            tests.check(checksum==expected,"checked at and unchecked access checksum");
            std::cout<<trial<<','<<name<<','<<operations<<','<<std::chrono::duration<double,std::milli>(end-start).count()<<','<<checksum<<'\n';
        };
        auto checked=[&](unsigned i)->const std::string& { return list[static_cast<std::ptrdiff_t>(i)]; };
        auto at=[&](unsigned i)->const std::string& { return list.at(static_cast<std::ptrdiff_t>(i)); };
        auto unchecked=[&](unsigned i)->const std::string& { return list.unchecked(i); };
        if(trial%2) { measure("checked_subscript",checked);measure("at",at);measure("unchecked",unchecked); }
        else { measure("unchecked",unchecked);measure("at",at);measure("checked_subscript",checked); }
    }
}
