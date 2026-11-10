# Q6 - Text ownership and operator rules

## Design and algorithms

Text owns one dynamically allocated null-terminated character array and stores its length. Construction rejects nullptr, computes strlen and allocates length+1 bytes. The Rule-of-Three copy constructor allocates an independent buffer; assignment first protects self-assignment, allocates/copies a replacement and only then releases the old buffer. Destruction uses delete[]. No public mutable raw buffer is exposed. c_str() returns a const pointer whose lifetime still depends on the owner; assignment/destruction invalidates an old pointer.

Concatenation pseudocode: check length addition plus terminator for size_t overflow; allocate one exact-size result; copy left characters then right characters; write/retain the final null byte; return the result. The direct + does not mutate either operand. += allocates a replacement, copies both operands before deleting old storage and therefore supports a+=a. Equality first compares lengths, then characters; != negates equality. Output writes the stored C string. The implementation represents null-terminated text, not arbitrary byte strings with embedded null characters, and does not interpret Unicode code points.

## Copy instrumentation and elision

Static educational counters record successful allocations, constructions and copy constructors without printing inside hot loops. copy_experiment.cpp snapshots counters immediately around a+b+c, before comparison temporaries can affect the observations. The actual default local build observed0 copy constructors and2 result allocations. With -fno-elide-constructors it observed2 copies and4 allocations. The copy-plus-equals chain observed3 copies and5 allocations in both builds. These are captured compiler/run observations, not hardcoded expected language-wide counts. The content is checked independently in every variant. C++17 mandatory prvalue elision remains even when optional NRVO is disabled; named result/parameter returns can still affect copy counts. No move constructor is added to obscure the requested Rule-of-Three exercise.

The alternative policy `Text operator+(Text left,const Text& right) { left+=right;return left; }` is demonstrated as plusOnCopy(), avoiding two conflicting operator+ definitions in one class. Copying lvalue a into its parameter and returning a named parameter can create additional copies when no move operation is available. std::string handles ownership/copying automatically and generally has move and capacity-reuse opportunities.

## Character-copy and storage costs

For lengths n1,n2,n3, direct a+b+c copies n1+n2 characters into the first concatenation and n1+n2+n3 into the second, plus terminators:2n1+2n2+n3+O(1). The intermediate and final result overlap in lifetime during the second addition, so peak extra result storage is O((n1+n2)+(n1+n2+n3)). This simplifies to O(n1+n2+n3) Big-O time/storage. Optional extra copies add linear work and storage with the same asymptotic order. Copy-on-+= adds copies of incoming/returned values as observed; it need not change Big-O. A capacity-aware std::string may reuse the left temporary and change constants/allocation counts. Exact operator+ allocation strategy is implementation-dependent.

Copy construction/assignment is O(n), concat O(n1+n2), comparison worst-case O(n), printing O(n), owned storage O(n+1). Self-assignment returns immediately in O(1). Arithmetic overflow guards do not provide unlimited memory: a representable but unavailable allocation may still throw bad_alloc.

## Operator rules

The operators ::,.,.*,?:,sizeof cannot be overloaded; their language-defined parsing/member-selection/type-size roles are fixed. Operator precedence, associativity and arity cannot be changed. A user-defined overload requires at least one class/enum operand; it cannot redefine int+int. Assignment operator= must be a nonstatic member (in C++17) and controls mutation of its left object. Friends are useful for symmetric nonmember operations and streams when private access is justified, but friendship is not required if public observers suffice. Do not overload an operator with surprising meaning merely because syntax permits it.

## Measurement and recommendation

benchmark.cpp performs100000 a+b+c operations per mode, with input lengths256,128,64 and the same output448 characters. It compares custom direct+, copy-plus-equals and std::string across five alternating trials at both optimization levels. Setup, full-content sample checks and output are outside timing; each timed result's size/first/last characters feed an observable verified checksum. Allocation, concatenation and destruction are timed together. Custom allocation counts are captured before the outside-timing verification; std::string allocations are explicitly not instrumented, not reported as zero. Custom counters add a small bookkeeping cost that std::string lacks, so the timing comparison is of these actual implementations rather than a pure allocator experiment. Raw trials/medians are preserved.

The correct Text and copy experiments are executed under ASan/LSan separately. Passing those tests is evidence for the exercised paths, not a proof of all possible memory behaviour. Prefer std::string for production SMS code. Writing Text teaches ownership, null termination, exception safety, temporary lifetimes and operator design. A specialized allocator, representation or externally required ABI could justify a custom string after profiling and clear requirements.
