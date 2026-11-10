# Viva preparation - DSA0110 Assignment 3 (CO3)

D Hoshea Paul | 192471019 | Slot B

Personally compile, execute and practise changing each program before submission. The PDF requires individual execution and a defence of the work; automated/browser runs do not certify that personal step. Read the prediction records before rerunning their programs. Console demonstrations omit test labels; --tests records actual checks on stderr.

## Q1 - Book constructors

Book owns fixed title/author arrays and a nonnegative copy count. Empty and donated constructors delegate to one full initialization/validation path. Member initializer lists initialize state directly. The three object-state diagrams are in Task01/explanation.md.

- When does the compiler supply Book()? When no user-declared constructor prevents implicit declaration, provided required member/base initialization is valid. A parameterized constructor does not also create a default constructor.
- Why initializer lists? They initialize directly, support const/reference members and avoid default construction followed by assignment.
- What controls member order? Declaration order, regardless of initializer-list order. The isolated -Wreorder example avoids uninitialized reads.
- Why explicit? It prevents surprising implicit conversions such as int-to-Book in a function argument; direct construction is still allowed.
- What if construction throws? No valid Book is produced. Already constructed members are cleaned automatically.
- Complexity? O(1) with these bounded fixed arrays; O(L) character copying if capacity/title length L is treated as variable. Unbounded std::string copies are not generally constant time.

Practise: (1) add an edition field through the delegating constructor without duplicating validation; (2) replace title storage with std::string and explain the complexity/size change.

## Q2 - Prescription ownership

Each correct Prescription owns a separate new[] medicine array. Construction creates it; copying duplicates the strings; assignment builds a replacement before deleting old storage; destruction uses delete[]. Self-assignment returns immediately. The supplied TODO retains its original program structure.

- Why did shallow copy share changes? A compiler-generated raw-pointer copy copies the address, not the allocation.
- Why can ASan say use-after-free rather than double-free? The second string-array destructor may read already freed metadata before attempting its second deletion.
- What is the Rule of Three? An owning destructor, copy constructor and copy assignment must implement a consistent resource policy.
- Construction versus assignment? Construction creates a new object's state; assignment replaces a live object's existing state and must release old resources safely.
- When are copies made? Initialization from an object, a by-value argument and some return-by-value situations. NRVO/mandatory prvalue elision can remove return copies.
- Value versus const-reference parameter? The lvalue value-argument test observed one copy; const reference observed none.
- Rule of Zero? vector<string> already manages resource lifetime and deep value copies, so no custom three functions are needed.
- Complexity? Deep/vector copy O(n+L) for n medicines and L characters, or O(n) with bounded names. Shallow pointer copy O(1) is not a correct independent owning copy.

Practise: (1) assign a larger prescription into a smaller one and explain replacement allocation; (2) implement the same interface with vector<string> and remove the ownership special members.

## Q3 - Dynamic auditorium

The map owns R row pointers and R separate character arrays. Booking/cancellation validates indices and changes occupancy only on successful state transitions. The destructor summarizes and releases every row then the pointer array. Copying is disabled. Constructor failure releases any rows already allocated.

- Why both delete and delete[]? delete ends a single heap event's lifetime; its destructor calls delete[] on each array allocated with new[]. These allocation forms must match.
- What destroys a block object? Scope exit, including normal unwinding. A heap object's pointer leaving scope does not call delete.
- Why no destructor arguments/overloads? C++ defines one no-argument destructor special-member signature per class.
- Does a destructor automatically solve missing delete? No. RAII needs an owner whose lifetime actually ends; an abandoned raw heap owner can still leak.
- What does zero rows do? No allocation, zero occupancy and rejected booking indices.
- What are allocated bytes? R*C char payload plus R*sizeof(char*) row pointers, excluding object/allocator metadata. Whole-process working set includes much more and is measured separately.
- Why did the leak test change? The first negative test produced no leak report. The later worker-thread version removes completed-thread roots and produced a real102-byte/four-allocation report. Absence of a diagnostic alone did not prove the original leak-free.
- Complexity? O(R*C) initialization/storage plus O(R) metadata/allocations; indexed access O(1). A contiguous block reduces allocation count/locality overhead.

Practise: (1) replace row allocations with one char[R*C] block and update the destructor/index formula; (2) implement move ownership transfer or keep copying explicitly deleted and explain the choice.

## Q4 - Money operators

Money stores signed integer paise. Checked arithmetic preserves exact cents within range. + and - produce new values; += updates and returns the left object. Negative values are permitted. Stream input parses a strict decimal token before committing it; malformed/overflow input sets failbit and leaves the target unchanged.

- Why not double? Many decimal fractions are not exact binary values; the million-addition experiment records the actual accumulated difference.
- Why does += return Money&? It supports chaining and identifies the updated object; + returns a new result without mutating operands.
- Why nonmember <<? ostream is on the left. It returns the same stream for chaining; friendship is one way to access private state.
- How is a+b called? The primary friend is operator+(a,b); the supplied member version is a.operator+(b).
- Why explicit conversion? It makes rupees/paise intent visible. With a nonexplicit converting constructor, a suitable nonmember can convert either side of100+m or m+100; member lookup cannot start from a plain int.
- Money*Money? Currency-squared is usually not a billing amount. Scalar multiplication needs its own units/rounding policy.
- LLONG_MIN output? Its positive magnitude cannot fit long long; unsigned modular arithmetic avoids signed abs overflow.
- Complexity? Arithmetic O(1); parsing O(L) token time/storage. Benchmarks isolate integer/double representation loops, not the full cost of guarded Money operators.

Practise: (1) add checked scalar multiplication by an integer quantity; (2) accept an optional Rs input prefix while preserving failbit and unchanged-target behaviour.

## Q5 - Increment and playlist access

Time stores total duration seconds, so minute/hour rollover follows division/modulus. Prefix increments and returns a reference; postfix saves and returns the prior value. Playlist's checked signed index returns mutable or const references and throws for invalid access; the caller prints the message.

- What is postfix's int? A dummy overload discriminator, not an increment amount.
- Why can postfix cost more? It must preserve an observed old value, but physical copying may be optimized away, especially if discarded. It is not universally slower.
- What happens after00:59:59?01:00:00. This duration does not wrap at24 hours.
- Why both [] overloads? Mutable access permits editing, while const access returns a const string reference.
- Why not print an error and return a fake track? It hides failure and can create invalid-reference semantics. Throwing lets the caller choose presentation.
- Does unchecked mean valid? No. The benchmark deliberately supplies only indices0..size-1; out-of-range unchecked access would be undefined.
- Complexity? O(1) increment/access/comparison, O(n+L) playlist storage. Checks add constants that may be optimized; measured results are workload-specific.

Practise: (1) implement a day-clock variant that wraps at24 hours and change the tests; (2) add a mutable at(i) while retaining const correctness and the same range policy.

## Q6 - Text and operator rules

Text owns length+1 chars including its terminator. Copying allocates independently. Assignment and += build replacements before releasing old storage. The direct + allocates an exact-size result and returns it. No move constructor is added, so the Rule-of-Three/copy-elision observations are visible.

- Why length+1? The final byte holds '\0'; strlen excludes it.
- Why self-assignment protection? Deleting the current buffer before reading the same source would destroy the data being copied.
- Why is a+=a safe here? Both source portions are copied before the old buffer is deleted.
- How many copies in a+b+c? Report the actual recorded build. Default local build observed0 copies/2 allocations; optional-elision-disabled build2 copies/4 allocations. The alternative observed3 copies/5 allocations. These are not universal constants.
- Which operators cannot be overloaded? ::,.,.*,?:,sizeof. Precedence/arity do not change, and an overload needs a class/enum operand.
- Why must assignment be a member? C++17 requires operator= to be a nonstatic member controlling its left object's state.
- Chained-copy complexity? Direct character work2n1+2n2+n3+O(1), with overlapping intermediate/final storage. Big-O is linear in total characters; extra copies change constants.
- Why prefer std::string? It supplies tested ownership, moves and capacity policies. A custom ABI/allocator/representation might justify a custom type after profiling.
- Sanitizer conclusion? No findings in exercised correct paths, not proof of all possible memory behaviour.

Practise: (1) add move construction/assignment and measure how the copy experiment changes; (2) compare direct+ with the +=-on-copy alternative using a different input-length mix.

## Q7 - Complex impedance

Complex stores finite real/imaginary doubles. It implements the arithmetic itself, with scaled division and checked finite results. Exact == stays separate from tolerant verification. RLC helpers validate physical values and handle ideal shorts/singular combinations explicitly.

- Why three constructors? Zero supports accumulation, real-only models resistance and a full pair models reactance/general impedance.
- Multiply/divide formulas? (a+jb)(c+jd)=(ac-bd)+j(ad+bc). Division multiplies by the conjugate and divides by c*c+d*d; the implementation scales to reduce overflow/underflow.
- Why separate nearlyEqual? Tolerance-based closeness can be nontransitive and should not silently replace exact equality semantics.
- What is omega?2*pi*f. ZR=R, ZL=j*omega*L, ZC=-j/(omega*C).
- Series versus parallel? Series adds impedances. Parallel adds reciprocal impedances, then inverts; the two-branch product-over-sum formula is also tested.
- A zero branch? An ideal short dominates parallel branches. Opposing nonzero admittances may make an open/singular equivalent with no finite result.
- How were results verified? Independent hand-derived scalar formulas, then248 arithmetic comparisons and circuit checks against std::complex, with documented absolute/relative tolerances.
- Remaining limitations? Floating rounding/cancellation, representable-range limits, platform-dependent long double and an ideal-component model. The direct product formula can overflow an intermediate.
- Complexity/recommendation? O(1) arithmetic/object storage with two components. Prefer std::complex plus application validation in production; retain the manual class for this design exercise.

Practise: (1) change frequency to60Hz and recompute the reference before running; (2) replace the two-branch product formula with reciprocal admittance and test extreme magnitudes without presuming numerical superiority.

## Personal rehearsal

Run the normal/TODO programs on Programiz yourself, reproduce a local warning, explain each pre-execution prediction, and inspect the raw sanitizer report rather than memorizing PASS. Re-run one benchmark at both flags and distinguish observations from Big-O. Practise at least two modifications above per question. Submit the report and exact repository link only after verifying the faculty's destination and accepting the repository-name discrepancy.
