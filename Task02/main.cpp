#include "prescription.hpp"
#include "../tools/checks.hpp"
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
