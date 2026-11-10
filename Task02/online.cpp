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
#include <vector>
std::string byValue(Prescription value) { value.set(0,"Temporary");return value.get(0); }
const std::string& byConstReference(const Prescription& value) { return value.get(0); }
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    Prescription p(2);p.set(0,"Paracetamol");p.set(1,"ORS");
    Prescription q=p;q.set(1,"Cetirizine");
    Prescription r(1);r=p;p.set(0,"Ibuprofen");
    std::cout << p.get(0) << ' ' << p.get(1) << " | " << q.get(0) << ' ' << q.get(1)
              << " | " << r.get(0) << ' ' << r.get(1) << '\n';
    tests.check(p.get(0)=="Ibuprofen" && q.get(0)=="Paracetamol" && r.get(0)=="Paracetamol","copies own independent medicine arrays");
    tests.check(q.get(1)=="Cetirizine" && p.get(1)=="ORS","copied medicine changes do not alter original");
    const auto before=Prescription::copyConstructions;
    Prescription::traceCopies=true;
    tests.check(byValue(p)=="Temporary" && p.get(0)=="Ibuprofen","pass-by-value modification is independent");
    const auto afterValue=Prescription::copyConstructions;
    tests.check(byConstReference(p)=="Ibuprofen","const-reference reads original");
    const auto afterReference=Prescription::copyConstructions;
    Prescription::traceCopies=false;
    tests.check(afterValue-before==1 && afterReference-afterValue==0,"value makes one copy; const reference makes none");
    std::cout << "Pass-by-value copies=" << afterValue-before << "; const-reference copies=" << afterReference-afterValue << '\n';
    p=p;tests.check(p.get(0)=="Ibuprofen" && p.size()==2,"self-assignment preserves data");
    Prescription empty(0),emptyCopy(empty);r=empty;
    tests.check(emptyCopy.size()==0 && r.size()==0,"empty copy construction and assignment");
    bool negative=false,index=false;
    try { Prescription invalid(-1); } catch(const std::invalid_argument&) { negative=true; }
    try { empty.get(0); } catch(const std::out_of_range&) { index=true; }
    tests.check(negative && index,"negative count and empty index rejected");
    std::vector<std::string> automatic={"Paracetamol","ORS"};auto copied=automatic;copied[0]="Changed";
    tests.check(automatic[0]=="Paracetamol" && copied[0]=="Changed","vector Rule-of-Zero copy independence");
}
