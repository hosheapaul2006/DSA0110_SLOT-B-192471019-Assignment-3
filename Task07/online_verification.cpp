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
