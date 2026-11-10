#include "money.hpp"
#include "../tools/checks.hpp"
#include <sstream>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    Money first(120,75),second(79,25),discount(20);
    Money bill=first+second-discount;
    std::cout << "Bill=" << bill << '\n';
    tests.check(bill.inPaise()==18000 && add(first,second)==Money(200),"operator and named addition agree");
    Money carry=Money(0,75)+Money(0,50);std::cout << "Carry=" << carry << '\n';
    tests.check(carry==Money(1,25),"paise carry into rupees");
    Money negative=Money(0,50)-Money(0,75);std::cout << "Negative=" << negative << '\n';
    tests.check(negative.inPaise()==-25 && negative<Money(0),"negative balances and comparison");
    Money accumulator(1);tests.check(&(accumulator+=Money(0,25))==&accumulator && accumulator==carry,"plus-equals returns updated self reference");
    std::ostringstream formatted;formatted<<Money::fromPaise(std::numeric_limits<long long>::min());
    tests.check(formatted.str()=="Rs -92233720368547758.08","minimum signed value formats without absolute-value overflow");
    Money read(99);std::istringstream valid("-12.3");valid>>read;
    tests.check(!valid.fail() && read.inPaise()==-1230,"signed decimal stream input");
    std::cout << "Read=" << read << '\n';
    for(const char* token:{"1.234","abc","12.","+","92233720368547758.08"}) {
        Money unchanged(7);std::istringstream bad(token);bad>>unchanged;
        tests.check(bad.fail() && unchanged==Money(7),"invalid input leaves target unchanged and sets failbit");
    }
    std::istringstream minimum("-92233720368547758.08");minimum>>read;
    tests.check(!minimum.fail() && read.inPaise()==std::numeric_limits<long long>::min(),"minimum signed input accepted");
    bool overflow=false,underflow=false;
    Money maximum=Money::fromPaise(std::numeric_limits<long long>::max());
    try { maximum+=Money::fromPaise(1); } catch(const std::overflow_error&) { overflow=true; }
    try { Money::fromPaise(std::numeric_limits<long long>::min())-Money::fromPaise(1); } catch(const std::overflow_error&) { underflow=true; }
    tests.check(overflow && underflow && maximum.inPaise()==std::numeric_limits<long long>::max(),"arithmetic overflow rejected without changing accumulator");
    std::ostringstream state;state<<std::setfill('*');state<<Money(2,5);state<<std::setw(3)<<1;
    tests.check(state.str()=="Rs 2.05**1","stream fill restored after Money output");
}
