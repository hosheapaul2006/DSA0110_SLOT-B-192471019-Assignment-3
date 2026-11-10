#include "complex.hpp"
#include "../tools/checks.hpp"
#include <iomanip>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    const double frequency=50,R=100,L=0.1,C=100e-6;
    const Complex zr=resistor(R),zl=inductor(L,frequency),zc=capacitor(C,frequency);
    const Complex series=zr+zl+zc,parallel=parallelBranches({zr,zl,zc});
    std::cout<<std::fixed<<std::setprecision(9)<<"R="<<R<<" ohm; L="<<L<<" H; C="<<C<<" F; f="<<frequency<<" Hz\n";
    std::cout<<"Resistor: "<<zr<<" ohm\nInductor: "<<zl<<" ohm\nCapacitor: "<<zc<<" ohm\n";
    std::cout<<"Series RLC: "<<series<<" ohm; magnitude="<<series.magnitude()<<"\nParallel RLC: "<<parallel<<" ohm; magnitude="<<parallel.magnitude()<<'\n';
    tests.check(nearlyEqual(series,Complex(100,-0.415062082481134770)),"series impedance matches independent hand-derived reference");
    tests.check(closeScalar(series.magnitude(),100.000861378951699),"series magnitude reference");
    tests.check(nearlyEqual(parallel,Complex(99.8280197492985963,4.14348257671117248)),"parallel RLC matches independent scalar-admittance reference");
    tests.check(closeScalar(parallel.magnitude(),99.9139728713149492),"parallel magnitude reference");
    tests.check(Complex()==Complex(0,0) && Complex(3)==Complex(3,0),"default and real-only constructors");
    tests.check(nearlyEqual(Complex(3,4)*Complex(2,-1),Complex(10,5)),"hand complex multiplication");
    tests.check(nearlyEqual(Complex(3,4)/Complex(2,-1),Complex(0.4,2.2)),"hand complex division");
    const Complex two=parallelTwo(Complex(100),Complex(100));
    tests.check(two==Complex(50),"supplied two-branch parallel formula");
    tests.check(parallelTwo(Complex(),Complex(5,2))==Complex() && parallelBranches({Complex(5),Complex()})==Complex(),"zero parallel impedance is an ideal short");
    bool zero=false,singular=false;
    try { Complex(1)/Complex(); } catch(const std::domain_error& e) { zero=true;std::cout<<"Rejected: "<<e.what()<<'\n'; }
    try { parallelTwo(Complex(0,1),Complex(0,-1)); } catch(const std::domain_error&) { singular=true; }
    tests.check(zero && singular,"division by zero and opposing-reactance singularity rejected");
    tests.check(!(Complex(1)==Complex(1+1e-12)) && nearlyEqual(Complex(1),Complex(1+1e-12)),"exact equality separate from approximate verification");
    tests.check(closeScalar(0,0.5e-9,1e-9,0) && !closeScalar(0,2e-9,1e-9,0),"absolute tolerance boundaries");
    tests.check(closeScalar(1e6,1e6+0.0005,0,1e-9) && !closeScalar(1e6,1e6+0.002,0,1e-9),"relative tolerance boundaries");
    int invalid=0;
    try { capacitor(0,50); } catch(const std::invalid_argument&) { ++invalid; }
    try { capacitor(-1,50); } catch(const std::invalid_argument&) { ++invalid; }
    try { inductor(0.1,0); } catch(const std::invalid_argument&) { ++invalid; }
    try { inductor(0.1,-50); } catch(const std::invalid_argument&) { ++invalid; }
    try { resistor(-1); } catch(const std::invalid_argument&) { ++invalid; }
    tests.check(invalid==5,"invalid component and frequency scenarios rejected");
}
