# Q7 executed tests

| ID | Scenario/input | Expected | Actual | Result |
|---|---|---|---|---|
| 1 | Default/real-only/full constructors | Correct finite components | Checked | PASS |
| 2 | R100,L0.1,C100microF,f50 | ZR100,ZLj31.415926536,ZC-j31.830988618 | Printed; references checked through circuit tests | PASS |
| 3 | Series RLC |100-j0.415062082481135,mag100.000861378952 | Matches independent scalar reference within tolerance | PASS |
| 4 | Parallel RLC |99.8280197492986+j4.14348257671117,mag99.913972871315 | Matches independent scalar and std-library references | PASS |
| 5 |(3+4j)*(2-j) and division |10+5j;0.4+2.2j | Checked | PASS |
| 6 | Two100ohm parallel branches |50ohms | Exact check | PASS |
| 7 | Zero branch | Ideal short0 | Checked before singular raw formula | PASS |
| 8 | Zero divisor/opposing reactances/empty branch set | Exceptions; no fabricated value | Checked | PASS |
| 9 | Exact vs approximate equality;abs/rel tolerance boundaries | Distinct documented behaviour | Checked | PASS |
| 10 | Negative/zero/nonfinite physical inputs | Rejected as documented | Checked | PASS |
| 11 |64 operand pairs vs std::complex |248 arithmetic results agree | All248 checked | PASS |
| 12 | Huge/tiny equal operands divided |1+0j without denominator-square overflow/underflow | Checked | PASS |
| 13 | Nonfinite component and unrepresentable product | Exceptions | Checked | PASS |

The hand-derived reference source is independent of custom/std::complex arithmetic. Actual values, sizeof measurements, check logs, compiler flags and source hashes are retained in outputs/.
