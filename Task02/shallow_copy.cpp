// Isolated negative test. Execute ONLY under AddressSanitizer.
#include <iostream>
#include <string>
class Prescription {
    int n;
    std::string* medicines;
public:
    explicit Prescription(int count):n(count),medicines(new std::string[count]) {}
    ~Prescription() { delete[] medicines; }
    void set(int i,const std::string& value) { if(i>=0 && i<n) medicines[i]=value; }
    const std::string& get(int i) const { return medicines[i]; }
    const void* storage() const { return medicines; }
};
int main() {
    Prescription original(2);original.set(0,"Paracetamol");
    Prescription copy=original;copy.set(0,"Changed");
    std::cout << "Shared storage=" << std::boolalpha << (original.storage()==copy.storage())
              << "; original now=" << original.get(0) << std::endl;
    // Both destructors attempt to release the same owning array: expected ASan failure.
}
