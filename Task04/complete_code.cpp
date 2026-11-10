#include <iomanip> 
#include <iostream> 
 
class Money { 
    long long paise; 
 
public: 
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {} 
 
    Money operator+(const Money& other) const
    {
        Money result;
        result.paise = paise + other.paise;
        return result;
    }
 
    bool operator==(const Money& other) const { return paise == other.paise; }
    friend std::ostream& operator<<(std::ostream& out, const Money& value)
    {
        const char previous = out.fill();
        const unsigned long long magnitude = value.paise < 0
            ? 0ULL - static_cast<unsigned long long>(value.paise)
            : static_cast<unsigned long long>(value.paise);
        out << "Rs " << (value.paise < 0 ? "-" : "") << magnitude / 100 << '.'
            << std::setfill('0') << std::setw(2) << magnitude % 100;
        out.fill(previous);
        return out;
    }
}; 
 
int main() 
{ 
    Money a(120, 75), b(79, 25); 
    Money c = a + b; 
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n'; 
}
