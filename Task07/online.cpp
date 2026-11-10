#ifndef CIRCUIT_COMPLEX_HPP
#define CIRCUIT_COMPLEX_HPP
#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <stdexcept>
class Complex {
    double re,im;
    static Complex arithmetic(long double real,long double imaginary) {
        const long double limit=std::numeric_limits<double>::max();
        if(!std::isfinite(real) || !std::isfinite(imaginary) || std::abs(real)>limit || std::abs(imaginary)>limit)
            throw std::overflow_error("complex result is not representable");
        return Complex(static_cast<double>(real),static_cast<double>(imaginary));
    }
public:
    Complex():Complex(0,0) {}
    explicit Complex(double real):Complex(real,0) {}
    Complex(double real,double imaginary):re(real),im(imaginary) {
        if(!std::isfinite(re) || !std::isfinite(im)) throw std::invalid_argument("complex components must be finite");
    }
    double real() const { return re; }
    double imag() const { return im; }
    double magnitude() const {
        const double result=std::hypot(re,im);
        if(!std::isfinite(result)) throw std::overflow_error("magnitude is not representable");
        return result;
    }
    bool operator==(const Complex& other) const { return re==other.re && im==other.im; }
    friend Complex operator+(const Complex& a,const Complex& b) { return arithmetic(static_cast<long double>(a.re)+b.re,static_cast<long double>(a.im)+b.im); }
    friend Complex operator-(const Complex& a,const Complex& b) { return arithmetic(static_cast<long double>(a.re)-b.re,static_cast<long double>(a.im)-b.im); }
    friend Complex operator*(const Complex& a,const Complex& b) {
        const long double ar=a.re,ai=a.im,br=b.re,bi=b.im;
        return arithmetic(ar*br-ai*bi,ar*bi+ai*br);
    }
    friend Complex operator/(const Complex& a,const Complex& b) {
        if(b.re==0 && b.im==0) throw std::domain_error("division by zero impedance");
        const long double scale=std::max(std::abs(b.re),std::abs(b.im));
        const long double br=b.re/scale,bi=b.im/scale,ar=a.re/scale,ai=a.im/scale;
        const long double denominator=br*br+bi*bi;
        return arithmetic((ar*br+ai*bi)/denominator,(ai*br-ar*bi)/denominator);
    }
    friend std::ostream& operator<<(std::ostream& out,const Complex& value) {
        return out<<value.re<<(value.im<0?" - j":" + j")<<std::abs(value.im);
    }
};
inline bool closeScalar(double a,double b,double absolute=1e-9,double relative=1e-9) {
    return std::abs(a-b)<=absolute+relative*std::max(std::abs(a),std::abs(b));
}
inline bool nearlyEqual(const Complex& a,const Complex& b,double absolute=1e-9,double relative=1e-9) {
    return closeScalar(a.real(),b.real(),absolute,relative) && closeScalar(a.imag(),b.imag(),absolute,relative);
}
inline double angularFrequency(double frequency) {
    if(!std::isfinite(frequency) || frequency<=0) throw std::invalid_argument("frequency must be finite and positive");
    const double result=2*std::acos(-1.0)*frequency;
    if(!std::isfinite(result)) throw std::overflow_error("angular frequency overflow");
    return result;
}
inline Complex resistor(double resistance) {
    if(!std::isfinite(resistance) || resistance<0) throw std::invalid_argument("invalid resistance");
    return Complex(resistance);
}
inline Complex inductor(double inductance,double frequency) {
    if(!std::isfinite(inductance) || inductance<0) throw std::invalid_argument("invalid inductance");
    const double reactance=angularFrequency(frequency)*inductance;
    if(!std::isfinite(reactance) || (inductance>0 && reactance==0)) throw std::overflow_error("inductor reactance out of range");
    return Complex(0,reactance);
}
inline Complex capacitor(double capacitance,double frequency) {
    if(!std::isfinite(capacitance) || capacitance<=0) throw std::invalid_argument("capacitance must be finite and positive");
    const double product=angularFrequency(frequency)*capacitance;
    if(!std::isfinite(product) || product==0) throw std::overflow_error("capacitor reactance out of range");
    const double reactance=1/product;
    if(!std::isfinite(reactance) || reactance==0) throw std::overflow_error("capacitor reactance out of range");
    return Complex(0,-reactance);
}
inline Complex parallelTwo(const Complex& a,const Complex& b) {
    if(a==Complex() || b==Complex()) return Complex(); // An ideal short dominates parallel branches.
    return (a*b)/(a+b); // Requested two-branch formula; zero sum throws.
}
inline Complex parallelBranches(std::initializer_list<Complex> branches) {
    if(branches.size()==0) throw std::invalid_argument("no parallel branches");
    Complex admittance;
    for(const Complex& branch:branches) { if(branch==Complex()) return Complex();admittance=admittance+Complex(1)/branch; }
    if(admittance==Complex()) throw std::domain_error("parallel admittance cancels; no finite impedance");
    return Complex(1)/admittance;
}
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
