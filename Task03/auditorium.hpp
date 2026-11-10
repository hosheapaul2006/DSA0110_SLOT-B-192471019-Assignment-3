#ifndef AUDITORIUM_HPP
#define AUDITORIUM_HPP
#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
class Auditorium {
    std::string name;
    int rows;
    int seatsPerRow;
    char** seats;
    std::size_t occupied=0;
    unsigned long long bookings=0,cancellations=0;
    bool messages;
    static int dimension(int value) { if(value<0) throw std::invalid_argument("negative seat-map dimension");return value; }
    void index(int row,int seat) const {
        if(row<0 || row>=rows || seat<0 || seat>=seatsPerRow) throw std::out_of_range("seat outside map");
    }
public:
    Auditorium(const std::string& label,int r,int perRow,bool trace=true)
      :name(label),rows(dimension(r)),seatsPerRow(dimension(perRow)),seats(nullptr),messages(trace) {
        if(rows && static_cast<std::size_t>(seatsPerRow)>std::numeric_limits<std::size_t>::max()/static_cast<std::size_t>(rows))
            throw std::length_error("seat-map size overflow");
        if(rows) seats=new char*[rows]{};
        try { for(int i=0;i<rows;++i) seats[i]=new char[seatsPerRow]{}; }
        catch(...) { for(int i=0;i<rows;++i) delete[] seats[i];delete[] seats;throw; }
        if(messages) std::cout << "Open " << name << " (" << rows << 'x' << seatsPerRow << ")\n";
    }
    Auditorium(const Auditorium&)=delete;
    Auditorium& operator=(const Auditorium&)=delete;
    ~Auditorium() {
        if(messages) std::cout << "Close " << name << ": occupied=" << occupied << ", bookings=" << bookings << ", cancellations=" << cancellations << '\n';
        for(int i=0;i<rows;++i) delete[] seats[i];
        delete[] seats;
    }
    bool book(int row,int seat) { index(row,seat);if(seats[row][seat]) return false;seats[row][seat]=1;++occupied;++bookings;return true; }
    bool cancel(int row,int seat) { index(row,seat);if(!seats[row][seat]) return false;seats[row][seat]=0;--occupied;++cancellations;return true; }
    std::size_t booked() const { return occupied; }
    std::size_t payloadBytes() const { return static_cast<std::size_t>(rows)*static_cast<std::size_t>(seatsPerRow)*sizeof(char); }
    std::size_t pointerBytes() const { return static_cast<std::size_t>(rows)*sizeof(char*); }
    std::size_t allocations() const { return rows?static_cast<std::size_t>(rows)+1:0; }
};
#endif
