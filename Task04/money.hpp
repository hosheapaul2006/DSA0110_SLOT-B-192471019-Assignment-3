#ifndef MONEY_HPP
#define MONEY_HPP
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
class Money {
    long long paise;
    static long long addChecked(long long a,long long b) {
        if((b>0 && a>std::numeric_limits<long long>::max()-b) ||
           (b<0 && a<std::numeric_limits<long long>::min()-b)) throw std::overflow_error("money addition overflow");
        return a+b;
    }
    static long long subtractChecked(long long a,long long b) {
        if((b>0 && a<std::numeric_limits<long long>::min()+b) ||
           (b<0 && a>std::numeric_limits<long long>::max()+b)) throw std::overflow_error("money subtraction overflow");
        return a-b;
    }
public:
    explicit Money(long long rupees=0,int fraction=0):paise(0) {
        if(rupees>std::numeric_limits<long long>::max()/100 || rupees<std::numeric_limits<long long>::min()/100)
            throw std::overflow_error("rupees overflow");
        paise=addChecked(rupees*100,fraction);
    }
    static Money fromPaise(long long value) { Money result;result.paise=value;return result; }
    long long inPaise() const { return paise; }
    Money& operator+=(const Money& other) { paise=addChecked(paise,other.paise);return *this; }
    friend Money operator+(const Money& a,const Money& b) { return fromPaise(addChecked(a.paise,b.paise)); }
    friend Money operator-(const Money& a,const Money& b) { return fromPaise(subtractChecked(a.paise,b.paise)); }
    bool operator==(const Money& other) const { return paise==other.paise; }
    bool operator<(const Money& other) const { return paise<other.paise; }
    friend std::ostream& operator<<(std::ostream& out,const Money& value) {
        const auto flags=out.flags();const char fill=out.fill();
        const auto magnitude=value.paise<0?0ULL-static_cast<unsigned long long>(value.paise):static_cast<unsigned long long>(value.paise);
        out << std::dec << "Rs " << (value.paise<0?"-":"") << magnitude/100 << '.'
            << std::setfill('0') << std::setw(2) << magnitude%100;
        out.flags(flags);out.fill(fill);return out;
    }
    friend std::istream& operator>>(std::istream& in,Money& value) {
        std::string token;if(!(in>>token)) return in;
        std::size_t i=0;bool negative=false;
        if(token[i]=='-' || token[i]=='+') { negative=token[i]=='-';++i; }
        const auto maximum=static_cast<unsigned long long>(std::numeric_limits<long long>::max());
        const auto limit=maximum+(negative?1ULL:0ULL);
        unsigned long long magnitude=0;std::size_t digits=0;
        auto append=[&](unsigned digit) {
            if(magnitude>(limit-digit)/10) return false;
            magnitude=magnitude*10+digit;return true;
        };
        bool valid=true;
        while(i<token.size() && std::isdigit(static_cast<unsigned char>(token[i]))) {
            valid=valid && append(static_cast<unsigned>(token[i]-'0'));++i;++digits;
        }
        if(digits==0) valid=false;
        unsigned fractionalDigits=0;
        if(i<token.size() && token[i]=='.') {
            ++i;
            while(i<token.size() && std::isdigit(static_cast<unsigned char>(token[i]))) {
                if(fractionalDigits==2) valid=false;
                valid=valid && append(static_cast<unsigned>(token[i]-'0'));++i;++fractionalDigits;
            }
            if(fractionalDigits==0) valid=false;
        }
        while(fractionalDigits<2) { valid=valid && append(0);++fractionalDigits; }
        if(i!=token.size() || !valid) { in.setstate(std::ios::failbit);return in; }
        const long long result=negative?(magnitude==maximum+1?std::numeric_limits<long long>::min():-static_cast<long long>(magnitude))
                                       :static_cast<long long>(magnitude);
        value=fromPaise(result);return in;
    }
};
inline Money add(const Money& a,const Money& b) { return a+b; }
#endif
