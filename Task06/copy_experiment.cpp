#include "text.hpp"
#include "../tools/checks.hpp"
int main(int argc,char** argv) {
    Tests tests(argc,argv);Text a("A"),b("B"),c("C");
    const auto copiesBefore=Text::copies,allocationsBefore=Text::allocations;
    Text result=a+b+c;
    const auto copiesAfter=Text::copies,allocationsAfter=Text::allocations;
    tests.check(result==Text("ABC"),"instrumented chained concatenation content");
    std::cout<<"a+b+c="<<result<<"; observed copy constructors="<<copiesAfter-copiesBefore
             <<"; observed allocations="<<allocationsAfter-allocationsBefore<<'\n';
    const auto alternativeCopies=Text::copies,alternativeAllocations=Text::allocations;
    Text alternative=plusOnCopy(plusOnCopy(a,b),c);
    const auto finalCopies=Text::copies,finalAllocations=Text::allocations;
    tests.check(alternative==result,"copy-plus-equals chain content");
    std::cout<<"copy-plus-equals="<<alternative<<"; observed copy constructors="<<finalCopies-alternativeCopies
             <<"; observed allocations="<<finalAllocations-alternativeAllocations<<'\n';
}
