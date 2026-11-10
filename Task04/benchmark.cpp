#include "../tools/checks.hpp"
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iomanip>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    constexpr int operations=1000000;
    std::array<long long,1024> paise{};std::array<double,1024> rupees{};
    std::uint32_t state=192471019u;long long block=0;
    for(unsigned i=0;i<paise.size();++i) { state=state*1664525u+1013904223u;paise[i]=1+state%99u;rupees[i]=paise[i]/100.0;block+=paise[i]; }
    long long expected=block*(operations/1024);for(int i=0;i<operations%1024;++i) expected+=paise[static_cast<unsigned>(i)];
    std::cout << std::setprecision(17) << "trial,design,operations,milliseconds,sum_rupees,error_rupees\n";
    for(int trial=1;trial<=5;++trial) {
        auto integer=[&]() {
            long long sum=0;const auto begin=std::chrono::steady_clock::now();
            for(int i=0;i<operations;++i) sum+=paise[static_cast<unsigned>(i)&1023u];
            const auto end=std::chrono::steady_clock::now();
            tests.check(sum==expected,"integer million-addition exact checksum");
            std::cout << trial << ",integer_paise," << operations << ',' << std::chrono::duration<double,std::milli>(end-begin).count()
                      << ',' << sum/100 << '.' << std::setfill('0') << std::setw(2) << sum%100 << ",0\n";
        };
        auto floating=[&]() {
            double sum=0;const auto begin=std::chrono::steady_clock::now();
            for(int i=0;i<operations;++i) sum+=rupees[static_cast<unsigned>(i)&1023u];
            const auto end=std::chrono::steady_clock::now();const double error=sum-expected/100.0;
            tests.check(std::abs(error)<0.0001,"double million-addition result within documented bound");
            std::cout << trial << ",double_rupees," << operations << ',' << std::chrono::duration<double,std::milli>(end-begin).count() << ',' << sum << ',' << error << '\n';
        };
        if(trial%2) { integer();floating(); } else { floating();integer(); }
    }
}
