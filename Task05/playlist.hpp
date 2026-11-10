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
