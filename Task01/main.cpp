#include "book.hpp"
#include "../tools/checks.hpp"
#include <string>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    Book empty,donation("Donated Algorithms"),complete("C++ Foundations","A. Reader",4);
    empty.print();donation.print();complete.print();
    tests.check(empty.count()==0 && empty.name().empty(),"default empty record");
    tests.check(donation.count()==1 && donation.name()=="Donated Algorithms","title-only donation");
    tests.check(complete.count()==4 && complete.name()=="C++ Foundations","full constructor");
    Book records[2];tests.check(records[0].count()==0 && records[1].count()==0,"default array construction");
    bool invalid=false;try { Book bad("Invalid","Author",-1); } catch(const std::invalid_argument& e) { invalid=true;std::cout << "Rejected: " << e.what() << '\n'; }
    tests.check(invalid,"negative copies rejected");
    Book zero("Withdrawn","Author",0);tests.check(zero.count()==0,"zero copies valid");
    Book boundary(std::string(127,'T'),std::string(95,'A'),1);
    tests.check(boundary.name().size()==127,"maximum fixed text lengths");
    bool longTitle=false,longAuthor=false;
    try { Book tooLong(std::string(128,'T')); } catch(const std::length_error&) { longTitle=true; }
    try { Book tooLong("Title",std::string(96,'A'),1); } catch(const std::length_error&) { longAuthor=true; }
    tests.check(longTitle && longAuthor,"overlong fields rejected without truncation");
    std::cout << "Book object bytes=" << sizeof(Book) << '\n';
}
