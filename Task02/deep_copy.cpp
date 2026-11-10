#include "prescription.hpp"
#include "../tools/checks.hpp"
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    Prescription original(3);original.set(0,"A");original.set(1,"B");original.set(2,"C");
    Prescription copy(original),assigned(0);assigned=original;
    original.set(0,"Changed");copy.set(2,"Independent");
    tests.check(copy.get(0)=="A" && assigned.get(0)=="A" && original.get(2)=="C","construction and resized assignment own separate values");
    tests.check(&(assigned=assigned)==&assigned && assigned.get(2)=="C","assignment returns self reference and protects self-assignment");
    std::cout << "Original=" << original.get(0) << "; copy=" << copy.get(0) << "; assigned=" << assigned.get(0) << '\n';
}
