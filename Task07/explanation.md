# Q7 - Complex-number impedance calculator

## Stage 1 - Interpret the problem

The hand-derived design/reference was committed before the custom implementation. Data are two finite double components. Required operations are construction, arithmetic, exact equality, rectangular output and magnitude. The measurable contract is agreement with independent scalar calculations at absTol1e-9 plus relTol1e-9 times the larger component magnitude, with explicit exceptions for zero divisors and unrepresentable finite results. Frequency and capacitance must be positive/finite; resistance/inductance must be nonnegative/finite. The demonstration uses R100ohms,L0.1H,C100microfarads,f50Hz. These are illustrative ideal lumped components, not a full physical circuit model.

## Stage 2 - Design and formulas

```text
Complex
  private re:double, im:double
  constructors: default(0,0); explicit real-only(r,0); full(r,i)
  public const real(),imag(),magnitude(), exact operator==
  nonmember friends +,-,*,/,<<
  arithmetic result check: finite and representable in double
  division check: denominator is not exactly0+0j
```

For a+jb and c+jd: sum=(a+c)+j(b+d); difference=(a-c)+j(b-d); product=(ac-bd)+j(ad+bc); quotient=((ac+bd)+j(bc-ad))/(c^2+d^2). Division scales both denominator components by max(abs(c),abs(d)), and also scales the numerator before computing the equivalent quotient. This avoids directly squaring a huge/tiny double denominator. Long-double intermediate precision/range is used where the implementation offers it, followed by a double representability check. It is not arbitrary-precision arithmetic and portable long-double formats vary.

Symmetric binary arithmetic is implemented as nonmember friends, supporting access to both private representations without modifying operands. Equality is a const member. Stream insertion is nonmember so ostream can be on the left. An explicit real-only constructor prevents surprising automatic numeric conversions; mixed expressions can name Complex(real) deliberately. The default value supports accumulation; real-only represents a resistor; the full constructor represents reactance or a general impedance. No public mutator can put nonfinite components into an existing object.

operator== compares components exactly, preserving ordinary equality semantics for finite values. nearlyEqual is a separate verification helper. Approximate closeness need not be transitive, so it is not silently substituted for equality. Tolerance boundary tests distinguish absolute error near zero from relative error at larger magnitudes. A very small nonzero impedance is not classified as zero merely because it is below a tolerance.

## Stage 3 - Hand calculation and verification

omega=2*pi*50=314.159265358979324 rad/s. XL=omega*0.1=31.4159265358979324ohms. XC=1/(omega*0.0001)=31.8309886183790672ohms.

Series Z=100+j(XL-XC)=100-j0.415062082481134770ohms; magnitude100.000861378951699.

For parallel branches, Y=G+jB where G=1/100=0.01 and B=omega*C-1/(omega*L)=-0.00041506208248113477. Inverting by conjugation gives Z=(G-jB)/(G^2+B^2)=99.8280197492985963+j4.14348257671117248ohms; magnitude99.9139728713149492. hand_calculation.cpp evaluates these independently derived scalar long-double formulas without either complex implementation. Its output was saved before implementing Complex; main checks against those recorded numerical constants.

The requested two-branch formula (Z1*Z2)/(Z1+Z2) is tested with two100ohm resistors, yielding50ohms. A zero branch is explicitly an ideal short and returns zero before a raw0/0 formula. Opposing nonzero reactances make the denominator/admittance exactly zero: no finite equivalent impedance is returned and a domain_error reports the singularity. Three-branch RLC uses reciprocal admittances, not an incorrect direct extension of the two-branch product formula. Invalid/overflowed physical reactance calculations are rejected rather than returning a fabricated finite value.

verification.cpp compares248 arithmetic results against std::complex over64 operand pairs, covering zero, real, imaginary, negative and widely scaled values; it skips only division by an exact zero. It separately tests huge/tiny scaled denominators, invalid nonfinite values, unrepresentable products, empty/singular branch sets and the full RLC alternative. The standard library is used only for this independent reference, never as the custom class implementation.

## Stage 4 - Defend the design and alternatives

The class has O(1) time per fixed-width arithmetic/comparison/magnitude and O(1) object/auxiliary storage. Both it and std::complex store two components; actual sizeof values are printed by verification.cpp instead of assuming an ABI. std::complex is mature, concise and benefits from carefully engineered numerical algorithms. It does not define this application's ideal-short, component-validity or exception policy; callers still need to handle singular circuits and finite-output requirements.

The custom class makes the teaching formulas and error contracts explicit. Its limitations include floating-point rounding/underflow, possible catastrophic cancellation near resonance, platform-dependent long-double precision, no component uncertainty/frequency-dependent loss model, and the direct two-branch formula's intermediate-overflow risk even if a mathematical final result might fit. These are reported limitations, not claims that scaling solves all numerical problems. Prefer std::complex with a small circuit-validation layer in production; retain the custom implementation for this first-principles design exercise.
