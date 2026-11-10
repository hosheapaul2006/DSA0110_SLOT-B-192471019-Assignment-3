#include "complex.hpp"
#include "../tools/checks.hpp"
#include <complex>
#include <limits>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    const Complex inputs[]={Complex(),Complex(1),Complex(0,1),Complex(0,-1),Complex(3,4),Complex(-2,5),Complex(1e-100,1e-100),Complex(1e100,1e100)};
    unsigned comparisons=0;
    for(const auto& a:inputs) for(const auto& b:inputs) {
        const std::complex<double> sa(a.real(),a.imag()),sb(b.real(),b.imag());
        auto compare=[&](const Complex& value,const std::complex<double>& reference,const char* label) {
            tests.check(nearlyEqual(value,Complex(reference.real(),reference.imag()),1e-200,1e-9),label);++comparisons;
        };
        compare(a+b,sa+sb,"std complex addition agreement");compare(a-b,sa-sb,"std complex subtraction agreement");
        compare(a*b,sa*sb,"std complex multiplication agreement");
        if(!(b==Complex())) compare(a/b,sa/sb,"std complex division agreement for nonzero divisor");
    }
    tests.check(nearlyEqual(Complex(1e308,1e308)/Complex(1e308,1e308),Complex(1)),"scaled division avoids huge denominator square overflow");
    tests.check(nearlyEqual(Complex(1e-300,1e-300)/Complex(1e-300,1e-300),Complex(1)),"scaled division avoids tiny denominator square underflow");
    bool invalid=false,overflow=false,open=false,empty=false;
    try { Complex bad(std::numeric_limits<double>::infinity(),0); } catch(const std::invalid_argument&) { invalid=true; }
    try { Complex(1e308,1e308)*Complex(1e308,1e308); } catch(const std::overflow_error&) { overflow=true; }
    try { parallelBranches({Complex(0,1),Complex(0,-1)}); } catch(const std::domain_error&) { open=true; }
    try { parallelBranches({}); } catch(const std::invalid_argument&) { empty=true; }
    tests.check(invalid && overflow && open && empty,"nonfinite input unrepresentable product and singular/empty parallel cases");
    const Complex r=resistor(100),l=inductor(0.1,50),c=capacitor(100e-6,50);
    const std::complex<double> sr(100,0),sl(0,2*std::acos(-1.0)*50*0.1),sc(0,-1/(2*std::acos(-1.0)*50*100e-6));
    const auto reference=1.0/(1.0/sr+1.0/sl+1.0/sc);const auto actual=parallelBranches({r,l,c});
    tests.check(nearlyEqual(actual,Complex(reference.real(),reference.imag())),"full parallel RLC agrees with standard library alternative");
    int rejected=0;
    try { angularFrequency(std::numeric_limits<double>::quiet_NaN()); } catch(const std::invalid_argument&) { ++rejected; }
    try { capacitor(std::numeric_limits<double>::infinity(),50); } catch(const std::invalid_argument&) { ++rejected; }
    tests.check(rejected==2,"nonfinite physical parameters rejected");
    std::cout<<"Arithmetic comparisons with std::complex: "<<comparisons<<"\nScaled division and circuit boundary checks completed\n";
    std::cout<<"sizeof(custom Complex)="<<sizeof(Complex)<<"; sizeof(std::complex<double>)="<<sizeof(std::complex<double>)<<'\n';
}
