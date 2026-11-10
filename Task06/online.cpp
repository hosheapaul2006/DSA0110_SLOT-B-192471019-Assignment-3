#ifndef OWNED_TEXT_HPP
#define OWNED_TEXT_HPP
#include <cstddef>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
class Text {
    std::size_t length;
    char* characters;
    struct Uninitialized {};
    static std::size_t checkedLength(const char* value) {
        if(!value) throw std::invalid_argument("null text input");
        return std::strlen(value);
    }
    static char* allocate(std::size_t size) {
        if(size==std::numeric_limits<std::size_t>::max()) throw std::length_error("text size overflow");
        char* result=new char[size+1];++allocations;return result;
    }
    Text(std::size_t count,Uninitialized):length(count),characters(allocate(count)) { characters[length]='\0';++constructions; }
public:
    inline static unsigned long long constructions=0,copies=0,allocations=0;
    explicit Text(const char* value=""):length(checkedLength(value)),characters(allocate(length)) {
        std::memcpy(characters,value,length+1);++constructions;
    }
    Text(const Text& other):length(other.length),characters(allocate(length)) {
        std::memcpy(characters,other.characters,length+1);++copies;++constructions;
    }
    Text& operator=(const Text& other) {
        if(this==&other) return *this;
        char* replacement=allocate(other.length);
        std::memcpy(replacement,other.characters,other.length+1);
        delete[] characters;characters=replacement;length=other.length;return *this;
    }
    ~Text() { delete[] characters; }
    std::size_t size() const { return length; }
    const char* c_str() const { return characters; }
    Text& operator+=(const Text& other) {
        if(length>std::numeric_limits<std::size_t>::max()-1-other.length) throw std::length_error("text concatenation overflow");
        const std::size_t combined=length+other.length;
        char* replacement=allocate(combined);
        std::memcpy(replacement,characters,length);
        std::memcpy(replacement+length,other.characters,other.length+1);
        delete[] characters;characters=replacement;length=combined;return *this;
    }
    friend Text operator+(const Text& a,const Text& b) {
        if(a.length>std::numeric_limits<std::size_t>::max()-1-b.length) throw std::length_error("text concatenation overflow");
        Text result(a.length+b.length,Uninitialized{});
        std::memcpy(result.characters,a.characters,a.length);
        std::memcpy(result.characters+a.length,b.characters,b.length);
        return result;
    }
    bool operator==(const Text& other) const { return length==other.length && std::memcmp(characters,other.characters,length)==0; }
    bool operator!=(const Text& other) const { return !(*this==other); }
    friend std::ostream& operator<<(std::ostream& out,const Text& value) { return out<<value.characters; }
};
inline Text plusOnCopy(Text left,const Text& right) { left+=right;return left; }
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
#include <sstream>
#include <string>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    Text empty,a("Hello"),space(" "),b("world");
    Text combined=a+space+b;std::cout<<"Message="<<combined<<'\n';
    tests.check(empty.size()==0 && empty==Text(""),"empty text is null terminated and equal");
    tests.check(Text("X").size()==1 && Text("X")!=Text("Y"),"single character equality and inequality");
    tests.check(combined==Text("Hello world") && combined.size()==11,"chained concatenation value and length");
    tests.check(a==Text("Hello") && b==Text("world"),"concatenation does not mutate operands");
    Text copy(a),assigned;assigned=a;copy+=Text("!");
    tests.check(a==Text("Hello") && copy==Text("Hello!") && assigned==a,"deep copy construction and assignment independence");
    tests.check(&(assigned=assigned)==&assigned && assigned==a,"self-assignment and returned reference");
    Text doubled("ab");doubled+=doubled;tests.check(doubled==Text("abab"),"self-concatenation before old storage release");
    std::string longInput(100000,'L');Text longMessage(longInput.c_str()),longCopy(longMessage);
    longMessage=Text("Short");tests.check(longCopy.size()==100000 && std::strlen(longCopy.c_str())==100000,"long message copy survives source reassignment");
    tests.check((a+empty)==a && (empty+a)==a,"empty operands in both concatenation positions");
    std::ostringstream printed;printed<<combined;tests.check(printed.str()=="Hello world","stream output");
    bool rejected=false;try { Text invalid(nullptr); } catch(const std::invalid_argument&) { rejected=true; }
    tests.check(rejected,"null character pointer rejected");
    Text alternative=plusOnCopy(plusOnCopy(a,space),b);
    tests.check(alternative==combined,"copy-plus-equals alternative matches direct concatenation");
    std::cout<<"Copy after independent edit="<<copy<<"; original="<<a<<'\n';
    std::cout<<"Long copy characters="<<longCopy.size()<<'\n';
}
