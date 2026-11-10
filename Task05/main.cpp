#include "playlist.hpp"
#include "../tools/checks.hpp"
#include <sstream>
#include <type_traits>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    Time t(0,59,58);Time a=t++;Time b=++t;
    std::cout << "a=t++: " << a << "; b=++t: " << b << "; t=" << t << '\n';
    tests.check(a==Time(0,59,58) && b==Time(1,0,0) && t==b,"postfix old value and prefix updated value");
    tests.check(a<b && !(b<a) && b==t,"earlier later and identical timestamps");
    Time minute(0,0,59);++minute;tests.check(minute==Time(0,1,0),"minute rollover");
    std::ostringstream printed;printed<<b;tests.check(printed.str()=="01:00:00","hour rollover formatting");
    Playlist playlist{"Opening","Finale"};playlist[0]="Opening (edited)";
    const Playlist& readOnly=playlist;
    tests.check(readOnly[0]=="Opening (edited)" && readOnly.at(1)=="Finale","mutable and const subscript access");
    tests.check((std::is_same_v<decltype(readOnly[0]),const std::string&>),"const playlist returns const reference");
    for(std::ptrdiff_t index:{-1,2}) {
        bool rejected=false;try { (void)playlist[index]; } catch(const std::out_of_range& e) { rejected=true;std::cout<<"Rejected index "<<index<<": "<<e.what()<<'\n'; }
        tests.check(rejected,"negative or size-equal index rejected");
    }
    Playlist empty;bool emptyRejected=false;
    try { (void)empty[0]; } catch(const std::out_of_range&) { emptyRejected=true; }
    tests.check(empty.size()==0 && emptyRejected,"empty playlist handled without invalid access");
    bool invalidTime=false;try { Time invalid(-1,0,0); } catch(const std::invalid_argument&) { invalidTime=true; }
    tests.check(invalidTime,"negative playback time rejected");
    Time limit=Time::fromSeconds(std::numeric_limits<unsigned long long>::max());bool overflow=false;
    try { ++limit; } catch(const std::overflow_error&) { overflow=true; }
    tests.check(overflow && limit.seconds()==std::numeric_limits<unsigned long long>::max(),"timer overflow leaves state unchanged");
    Time play(0,0,58);
    for(std::ptrdiff_t i=0;i<static_cast<std::ptrdiff_t>(playlist.size());++i) { std::cout<<"Playing "<<readOnly[i]<<" at "<<play<<'\n';++play;++play; }
}
