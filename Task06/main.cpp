#include "text.hpp"
#include "../tools/checks.hpp"
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
