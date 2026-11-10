# Q5 - Playback time and playlist operators

## Design and pseudocode

Time stores total seconds as an unsigned long long playback duration. Its constructor rejects negative fields and minute/second values outside0..59. Hours need not wrap at24 because this represents duration, not a day clock. Prefix checks the representable maximum, increments total and returns *this by reference. Postfix stores the old Time, calls prefix, then returns the old value. Formatting divides total into hours, minutes and seconds. Minute/hour rollover therefore requires no separate carry branches.

Playlist owns vector<string> tracks. Its signed index policy rejects i<0 or i>=size with out_of_range. Nonconst [] returns string& for editing; const [] returns const string& for reading without allowing mutation. at(i) explicitly delegates to the same checked access. unchecked(i) is a separate comparison interface with the precondition i<size, used only with valid benchmark indices. The independent simulation catches errors at the caller and prints a meaningful message rather than returning a fabricated track.

## Prefix, postfix and prediction

The unused int parameter distinguishes postfix operator++(int) from prefix operator++(). It does not specify how far to advance. Prefix returns the updated object reference; postfix must preserve the prior value when its value is observed, normally by returning a value copy. Copy elision and optimization can remove physical copying, especially when a postfix result is discarded. Thus the PDF's blanket suggestion that postfix is always slower is not a universal timing fact. ++i is a useful class-type default when the old value is unnecessary, but measurements decide whether it matters here.

For the supplied Counter, c=5; a=c++ stores5 and changes c to6; b=++c changes c to7 and copies7 into b. The exact precommitted output5 7 7 matched. The original prediction and source remain unchanged.

## Range policy and alternatives

The PDF proposes checked [] that prints an error directly. Printing alone cannot safely provide a valid reference for an invalid index. This design throws; the caller catches and reports it, so containers stay independent of UI/output choices and no dangling/fabricated fallback reference is used. at(i) communicates checked access explicitly and throws the same exception. An unchecked built-in/vector-style interface omits the guard but invalid access is undefined behaviour, so the unchecked benchmark never uses negative/size-equal indices.

All three access interfaces are O(1); reference return uses O(1) extra space. The signed guard logically checks both lower and upper bounds; the compiler may combine checks, so no exact machine instruction count is assumed. Playlist storage is O(n+L) for n strings and L total characters. Time increment/comparison is O(1) time/space. Printing a name takes its character length; it is outside access timing.

## Benchmarks and evaluation

The increment benchmark performs10000000 operations over1024 independent Time objects, indexed by a reproducible pre-generated4096-entry sequence. This prevents a simple one-counter loop from collapsing into a single final addition. Five trials at -O0 and -O2 compare discarded prefix/postfix results, plus observed-return variants normalized to the same old-value checksum. Every final timer is checked against independently counted visits; value-use checksums are verified outside timing. No per-iteration IO or volatile write is timed. Returned-value variants are additional workloads, not a claim that prefix/postfix return identical semantics.

The access benchmark performs10000000 accesses to the same1024 track strings through checked [], at and unchecked interfaces. It uses the same valid index stream, expected length checksum and five alternating-order trials at both flags. Setup, expectation calculation and printing are outside timing; access and reading string length are inside. at delegates to the same guard, so optimization may produce identical implementations. Raw trials and medians are in outputs and benchmark_results.md. Cache-friendly repeated inputs, compiler transformation and scheduler noise limit these results; no universal postfix/check-cost claim is made.
