// Independent scalar evaluation of the hand-derived circuit equations.
// This program does not use the custom Complex class or std::complex.
#include <cmath>
#include <iomanip>
#include <iostream>
int main() {
    const long double pi=3.141592653589793238462643383279502884L;
    const long double frequency=50,R=100,L=0.1L,C=0.0001L;
    const long double omega=2*pi*frequency,XL=omega*L,XC=1/(omega*C);
    const long double seriesX=XL-XC;
    const long double G=1/R,B=omega*C-1/(omega*L),denominator=G*G+B*B;
    const long double parallelReal=G/denominator,parallelImag=-B/denominator;
    std::cout<<std::setprecision(18)<<"omega="<<omega<<"\nXL="<<XL<<"\nXC="<<XC
             <<"\nseries_real="<<R<<"\nseries_imag="<<seriesX<<"\nseries_magnitude="<<std::hypot(R,seriesX)
             <<"\nparallel_real="<<parallelReal<<"\nparallel_imag="<<parallelImag
             <<"\nparallel_magnitude="<<std::hypot(parallelReal,parallelImag)<<'\n';
}
