#ifndef LIBRARY_BOOK_HPP
#define LIBRARY_BOOK_HPP
#include <array>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>
class Book {
    std::array<char,128> title{};
    std::array<char,96> author{};
    int copies;
    template<std::size_t N>
    static void store(std::array<char,N>& destination,std::string_view value) {
        if(value.size()>=N) throw std::length_error("book text exceeds fixed capacity");
        if(!value.empty()) std::memcpy(destination.data(),value.data(),value.size());
        destination[value.size()]='\0';
    }
public:
    Book():Book("","",0) {}
    explicit Book(std::string_view name):Book(name,"Donated",1) {}
    Book(std::string_view name,std::string_view writer,int count):title{},author{},copies(count) {
        if(count<0) throw std::invalid_argument("negative copy count");
        store(title,name);store(author,writer);
    }
    int count() const { return copies; }
    std::string_view name() const { return title.data(); }
    void print() const { std::cout << "Title=\"" << title.data() << "\" Author=\"" << author.data() << "\" Copies=" << copies << '\n'; }
};
#endif
#ifndef COURSEWORK_CHECKS_HPP
#define COURSEWORK_CHECKS_HPP
#include <iostream>
#include <stdexcept>
#include <string>
class Tests {
    bool logging;
    unsigned count=0;
public:
    Tests(int argc,char** argv):logging(argc>1 && std::string(argv[1])=="--tests") {}
    void check(bool condition,const char* scenario) {
        ++count;
        if(logging) std::cerr << "CHECK|" << scenario << '|' << (condition?"PASS":"FAIL") << '\n';
        if(!condition) throw std::runtime_error(scenario);
    }
    ~Tests() { if(logging) std::cerr << "TEST_COUNT " << count << '\n'; }
};
#endif
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
