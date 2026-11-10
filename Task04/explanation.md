# Q4 - Money arithmetic and streams

## Design, validation and algorithm

Money owns one signed long long paise value. Its explicit constructor interprets rupees*100+fraction arithmetically; fromPaise names an already scaled amount. Negative balances are permitted and print as Rs -0.25. The full implementation checks signed addition, subtraction and rupee scaling before arithmetic, throwing overflow_error without partially updating an accumulator. Even LLONG_MIN formats without calling abs on an unrepresentable positive magnitude: unsigned modular conversion obtains the magnitude safely.

Arithmetic pseudocode: reject overflow using limits and sign-aware comparisons; create a new value for + or -; for += assign the checked sum and return *this. Bill calculation is `first+second-discount`. The equivalent named addition is add(first,second), which the executed test compares to the operator result. Integer values represent every paise exactly within the signed64-bit range; this is bounded precision, not unlimited arithmetic.

Input accepts a signed decimal token such as -12.30,12,12.3, with one or two fractional digits if a decimal point is present. It rejects malformed tokens, extra fractional digits and values outside the signed paise range. It accumulates unsigned digits with overflow checks, then commits the result only on complete success. Failure sets failbit and leaves the target unchanged. The caller can clear the stream and decide how to report an error. Output includes Rs and exactly two fractional digits; stream flags/fill are restored. Input intentionally expects a bare amount, not the output's Rs prefix.

## Operator choices and translation

Nonmember friends + and - treat both operands symmetrically. Comparisons are const members. += is a member because it mutates its left operand and returns Money& for chaining. + returns a new Money because neither operand should change. `a+b` calls operator+(a,b) for this friend implementation; the supplied TODO deliberately uses a.operator+(b), as required. Stream insertion must support an ostream on the left, so it is a nonmember; it returns ostream& for chaining. Friendship allows direct access to representation, though public observers could implement output without friendship.

The primary constructor is explicit to keep units clear, so bare 100 is not silently converted in either operand position. If a nonexplicit Money(long long rupees) conversion is deliberately offered, a nonmember friend operator+(Money,Money) can convert either operand and support both100+m and m+100. A member operator+(Money) supports m+100 but cannot make an ordinary integer left operand perform member lookup on Money. Named functions can express the same policy without operator syntax. All arithmetic forms have O(1) time/auxiliary storage. Money*Money has currency-squared units and is intentionally omitted; a separately designed scalar multiplier could make sense with a documented rounding policy.

## Supplied TODO and limits

complete_code.cpp fills only the two TODO regions from Reference/Q4_TODO_original.txt and prints exactly Rs200.00 then true. It also formats negative values safely in its output operator. The supplied constructor's rupees*100+p and supplied-style + require representable arithmetic; the exercise's fixed positive values satisfy this. Its unchanged constructor is not advertised as a general overflow-safe API. The independent Money class implements those additional guards and boundary tests. A separate carry test checks75+50 paise=125.

## Million-addition experiment

benchmark.cpp generates1024 bounded paise values and equivalent doubles, then adds1000000 entries in the same cyclic order. An independent block-sum formula supplies the exact integer expectation. Five alternating-order trials are run under -O0 and -O2. The checksum is observable; IO and setup are outside timing. The hot loops use raw long long paise and double rupees to isolate representation arithmetic, not to imply that checked Money operators have no guard cost. The integer sum is exact; double output/error is printed with17 significant digits against the corresponding rounded double representation of the exact sum. Actual numeric results and timing medians are retained in outputs and benchmark_results.md. Repeated cache-friendly inputs and scheduler/optimizer effects limit generalization.

Parsing a token of length L takes O(L) time and O(L) token storage. Fixed-width arithmetic, comparison and formatting take O(1) time/space. Prefer integer paise for this billing task; floating point remains useful for scientific measurements but needs an explicit financial rounding policy.
