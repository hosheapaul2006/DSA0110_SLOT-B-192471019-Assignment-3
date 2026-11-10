# Q7 design and independent hand-derived reference

## Stage 1 - Measurable requirements

Use R=100 ohms,L=0.1 henry,C=100 microfarads,f=50 hertz. Default construction
must yield0+0j; real-only construction must yield r+0j; full construction must
retain both finite components. Arithmetic must match independently calculated
real/imaginary results within absTol=1e-9 plus relTol=1e-9 times the larger
component magnitude. Equality itself remains exact; approximate comparison is
an explicit verification function, not a nontransitive operator== policy.
Undefined division and singular finite-impedance combinations must throw rather
than return a made-up number. Negative/nonfinite R/L,nonpositive/nonfinite C or
frequency must be rejected. Zero R or L may physically create a short branch.

## Stage 2 - Formulas and class plan

```text
Complex
  private: real double, imaginary double
  constructors: default; explicit(real); (real,imaginary)
  const observers: real(),imag(),magnitude()
  friends: +,-,*,/,<<
  member: exact ==
  validation: finite inputs; representable arithmetic; nonzero divisor

(a+jb)+(c+jd) = (a+c)+j(b+d)
(a+jb)-(c+jd) = (a-c)+j(b-d)
(a+jb)*(c+jd) = (ac-bd)+j(ad+bc)
(a+jb)/(c+jd) = ((ac+bd)+j(bc-ad))/(c*c+d*d)
```

Implement division with scaled components to avoid directly squaring tiny or
huge double denominators. Retain the mathematical formula above for the proof.
Use long-double intermediates where available, then check representability as
double. std::complex is a verification alternative only, not the implementation.

## Independent hand calculations

omega=2*pi*50. Z_R=100, Z_L=j*(omega*0.1), Z_C=-j/(omega*0.0001).
Series: real=100, imaginary=XL-XC, magnitude=sqrt(100^2+(XL-XC)^2).
Parallel admittance: Y=G+jB, G=1/100, B=omega*C-1/(omega*L).
Invert by conjugation: Z=(G-jB)/(G^2+B^2).
These expressions do not call the custom complex operators. hand_calculation.cpp
evaluates the hand-derived scalar expressions in long double and records the
numeric reference before the custom class is implemented. Simple exact examples
also verify(3+4j)*(2-j)=10+5j and(3+4j)/(2-j)=0.4+2.2j.

For two branches, test(Z1*Z2)/(Z1+Z2). Handle a zero branch explicitly as a short
before applying a formula whose raw0/0 case would be undefined. Nonzero opposing
reactances whose sum/admittance cancels represent an open/singular equivalent;
report that no finite impedance is available.
