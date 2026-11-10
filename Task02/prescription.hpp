#ifndef PRESCRIPTION_HPP
#define PRESCRIPTION_HPP
#include <iostream>
#include <stdexcept>
#include <string>
class Prescription {
    int n;
    std::string* medicines;
    static int checkedCount(int count) {
        if(count<0) throw std::invalid_argument("negative medicine count");
        return count;
    }
    static std::string* duplicate(const std::string* source,int count) {
        if(count==0) return nullptr;
        std::string* result=new std::string[count];
        try { for(int i=0;i<count;++i) result[i]=source[i]; }
        catch(...) { delete[] result;throw; }
        return result;
    }
    void checkIndex(int i) const { if(i<0 || i>=n) throw std::out_of_range("medicine index"); }
public:
    inline static unsigned long long copyConstructions=0;
    inline static bool traceCopies=false;
    explicit Prescription(int count):n(checkedCount(count)),medicines(n?new std::string[n]:nullptr) {}
    Prescription(const Prescription& other):n(other.n),medicines(duplicate(other.medicines,other.n)) {
        ++copyConstructions;
        if(traceCopies) std::cout << "Copy constructor: " << n << " medicines\n";
    }
    Prescription& operator=(const Prescription& other) {
        if(this==&other) return *this;
        std::string* replacement=duplicate(other.medicines,other.n);
        delete[] medicines;medicines=replacement;n=other.n;
        return *this;
    }
    ~Prescription() { delete[] medicines; }
    int size() const { return n; }
    void set(int i,const std::string& medicine) { checkIndex(i);medicines[i]=medicine; }
    const std::string& get(int i) const { checkIndex(i);return medicines[i]; }
};
#endif
