# Q2 - Prescription copying and ownership

## Design and algorithms

Prescription owns n medicine strings in an array allocated by new[]. The independent implementation rejects negative counts and out-of-range indices. Zero medicines use a null pointer, which delete[] safely accepts. A helper duplicates the array, copies every string, and releases a partially copied replacement if a string allocation throws.

Copy construction initializes a new object with a new allocation. Copy assignment first checks identity (`this==&other`), builds a complete replacement, then releases the old allocation and commits the new pointer/count. This protects self-assignment and preserves the target if copying throws. The destructor uses delete[] exactly once for its own allocation. These are the Rule of Three: destructor, copy constructor and copy assignment must agree on the ownership policy.

Pseudocode: COPY: allocate n strings; copy indices0..n-1; clean up on failure. ASSIGN: if same object return self; duplicate source; delete old array; install replacement; return self. DESTROY: delete[] owned array.

## Memory diagrams

```text
Shallow copy (isolated unsafe demonstration)
 original [n=2, pointer ------+]
 copy     [n=2, pointer ------+----> allocation A [medicine0][medicine1]]
 Two owners both delete[] A. Mutations are shared; final deletion is invalid.

Deep copy (correct implementation)
 original [n=2, pointer ----------> allocation A [Paracetamol][ORS]]
 copy     [n=2, pointer ----------> allocation B [Paracetamol][ORS]]
 assigned [n=2, pointer ----------> allocation C [Paracetamol][ORS]]
 Each owner deletes its own array. Updating B does not change A or C.
```

A pointer member's compiler-generated copy copies only the address. It does not invent ownership or allocate a second array. For string arrays, the second destructor can first read freed array/string metadata, so ASan may diagnose heap-use-after-free before it reaches the eventual second delete. Both indicate broken shared ownership. shallow_copy.cpp is compiled locally but executed only in the isolated sanitizer job. without_copy_constructor.cpp removes only the copy constructor from the completed supplied program, retaining its other code, for the requested negative experiment.

## Supplied TODO preservation

complete_code.cpp fills only TODO1,TODO2,TODO3 of Reference/Q2_TODO_original.txt. It produces the exact required line: Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS. No validation or logging is inserted into the supplied main, constructor or accessors. Its accessor precondition is a valid index and its supplied constructor expects a nonnegative count. The independent class supplies defensive validation without altering that exercise. Correct and negative sanitizer outputs are recorded separately; local normal compilation alone does not prove memory safety.

## Copy counts and measurement

The independent copy constructor counts successful copies and can print a message. Passing an existing lvalue by value makes one real copy in the executed test; passing it by const reference makes none. Typical copy-construction contexts are initialization from another object, a by-value parameter and returning an object by value. Return-value optimization and C++17 guaranteed prvalue elision mean the last case does not always invoke a copy constructor. Assignment to an already constructed object calls assignment instead.

The benchmark copies10,1000,100000 medicines with10000,200,5 repetitions respectively in each of five trials. Every string length is observed in a checksum. Allocation, copying, traversal and destruction are timed together, so dividing by repetitions estimates a complete copy lifetime, not isolated allocation latency. The input and checksum expectations are identical across optimization levels. See outputs/benchmark_O0_actual.txt and benchmark_O2_actual.txt for actual times; no improvement is inferred from Big-O alone.

## Alternatives and evaluation

With n medicine names and total L characters, deep copying generally costs O(n+L) time/storage; with bounded-length names this is O(n), as in the assignment's element-count model. A shallow pointer copy is O(1) time and additional pointer storage, but is unsafe for two independent raw owners. Its apparent speed is not a valid ownership implementation.

`vector<string> medicines;` needs no custom destructor/copy constructor/assignment. Its value copy owns independent strings and costs O(n+L), with O(n+L) additional storage. This is the Rule of Zero. The main program verifies a vector copy's independence. Compiler-generated copies suffice for value members such as string/vector or non-owning pointers with a valid lifetime policy; they do not suffice for an independently owning raw pointer. Prefer vector for application code. Implementing the raw owner teaches resource lifetime and exception-safe copying.
