#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP
#include <cstddef>
#include <iomanip>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
class Time {
    unsigned long long total;
public:
    Time(int h=0,int m=0,int s=0):total(0) {
        if(h<0 || m<0 || m>=60 || s<0 || s>=60) throw std::invalid_argument("invalid playback time");
        total=static_cast<unsigned long long>(h)*3600+static_cast<unsigned>(m)*60+static_cast<unsigned>(s);
    }
    static Time fromSeconds(unsigned long long value) { Time time;time.total=value;return time; }
    unsigned long long seconds() const { return total; }
    Time& operator++() {
        if(total==std::numeric_limits<unsigned long long>::max()) throw std::overflow_error("playback time overflow");
        ++total;return *this;
    }
    Time operator++(int) { Time previous=*this;++*this;return previous; }
    bool operator==(const Time& other) const { return total==other.total; }
    bool operator<(const Time& other) const { return total<other.total; }
    friend std::ostream& operator<<(std::ostream& out,const Time& value) {
        const char oldFill=out.fill();out<<std::setfill('0')<<std::setw(2)<<value.total/3600<<':'
            <<std::setw(2)<<value.total/60%60<<':'<<std::setw(2)<<value.total%60;
        out.fill(oldFill);return out;
    }
};
class Playlist {
    std::vector<std::string> tracks;
    std::size_t checked(std::ptrdiff_t index) const {
        if(index<0 || static_cast<std::size_t>(index)>=tracks.size()) throw std::out_of_range("playlist index out of range");
        return static_cast<std::size_t>(index);
    }
public:
    Playlist()=default;
    Playlist(std::initializer_list<std::string> values):tracks(values) {}
    std::size_t size() const { return tracks.size(); }
    void add(const std::string& title) { tracks.push_back(title); }
    std::string& operator[](std::ptrdiff_t index) { return tracks[checked(index)]; }
    const std::string& operator[](std::ptrdiff_t index) const { return tracks[checked(index)]; }
    const std::string& at(std::ptrdiff_t index) const { return (*this)[index]; }
    const std::string& unchecked(std::size_t index) const { return tracks[index]; }
};
#endif
#ifndef COURSEWORK_CHECKS_HPP
#define COURSEWORK_CHECKS_HPP
#include <iostream>
#include <stdexcept>
#include <string>
class Tests {
    bool logging;
    unsigned count=0;
public:
    Tests(int argc,char** argv):logging(argc>1 && std::string(argv[1])=="--tests") {}
    void check(bool condition,const char* scenario) {
        ++count;
        if(logging) std::cerr << "CHECK|" << scenario << '|' << (condition?"PASS":"FAIL") << '\n';
        if(!condition) throw std::runtime_error(scenario);
    }
    ~Tests() { if(logging) std::cerr << "TEST_COUNT " << count << '\n'; }
};
#endif
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
