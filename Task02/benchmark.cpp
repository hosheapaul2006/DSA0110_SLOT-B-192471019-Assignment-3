#include "prescription.hpp"
#include "../tools/checks.hpp"
#include <chrono>
#include <cstdint>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    std::cout << "trial,medicines,copies,milliseconds,total_characters_observed\n";
    for(int count:{10,1000,100000}) {
        Prescription original(count);
        for(int i=0;i<count;++i) original.set(i,"Medicine-"+std::to_string(i));
        const int repeats=count==10?10000:count==1000?200:5;
        std::uint64_t expected=0;
        for(int i=0;i<count;++i) expected+=original.get(i).size();
        for(int trial=1;trial<=5;++trial) {
            std::uint64_t observed=0;
            const auto begin=std::chrono::steady_clock::now();
            for(int repeat=0;repeat<repeats;++repeat) {
                Prescription copy(original);
                for(int i=0;i<count;++i) observed+=copy.get(i).size();
            }
            const auto end=std::chrono::steady_clock::now();
            tests.check(observed==expected*static_cast<unsigned>(repeats),"copied prescription content checksum");
            std::cout << trial << ',' << count << ',' << repeats << ','
                      << std::chrono::duration<double,std::milli>(end-begin).count() << ',' << observed << '\n';
        }
    }
}
