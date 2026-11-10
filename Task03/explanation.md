# Q3 - Auditorium dynamic construction and destruction

## Design, ownership and algorithm

Auditorium owns an array of row pointers and one char array per row. Constructor initialization validates nonnegative dimensions and checks the cell-count product before allocating. Pointer entries start as null; if any row allocation throws, the constructor catches the failure, deletes all previously allocated rows and the pointer array, then rethrows. A zero-row map uses nullptr and performs no allocation. Booking/cancellation check both indices, change only the requested state and update successful-operation statistics; duplicate operations return false.

The destructor prints its closing summary, deletes every row with delete[], then deletes the row-pointer array with delete[]. Copy construction and assignment are deleted, preventing two owners of the same seat map. No move/ownership transfer interface is offered. Pseudocode: validate; allocate R pointers; allocate/zero C chars per row; on book set an unbooked char; on cancel clear a booked char; on destruction delete each row and then the pointers. RAII means storage is released whenever the fully constructed object's lifetime ends, including stack unwinding. A raw heap object still needs its lifetime ended with delete; a destructor cannot clean an object whose owner never invokes it.

## Memory diagram and alternatives

```text
event [rows=2, seatsPerRow=3, seats --+]
                                   +--> row pointers [p0][p1]
                                                      |   |
                                            row0 [0][1][0] |
                                            row1 [0][0][1]<+
```

The row allocations are separate and not guaranteed adjacent. For R>0 there are R+1 new[] calls; requested character payload is R*C*sizeof(char), plus R*sizeof(char*) for row pointers. These requested byte counts exclude allocator bookkeeping, alignment, the object and its name. memory.cpp actually constructs and touches all cells of10x20,100x200,1000x2000 maps. Its process resident bytes come from the Windows working-set API (or /proc/self/status on Linux). That whole-process measurement includes executable/runtime/allocator memory and retained allocations; it is not the map payload and must not be substituted for it. A zero return would mean unavailable measurement, not zero process memory.

Alternative contiguous block pseudocode: allocate char[R*C]{}; access block[r*C+s]; delete[] block. It has one allocation for a nonempty map, no row-pointer array and predictable locality. vector<vector<char>>(R,vector<char>(C)) manages row lifetimes automatically and costs typically one outer allocation plus R nonempty row allocations, plus per-vector object/capacity metadata. Contiguous vector<char> combines automatic ownership and one block. All have O(1) indexed access and O(R*C) initialization/payload storage; row pointers/vector rows add O(R) metadata. The destructor releases O(R) allocations; initialization dominates O(R*C) when rows are nonempty. Prefer contiguous RAII storage in application code; manual rows satisfy the required exercise and demonstrate ownership.

## Lifetimes and diagnostic experiments

The main event constructs first and dies last. The nested event dies at block exit. The heap event dies exactly at explicit delete, not when its pointer variable goes out of scope. Constructor/destructor messages and closing counts are actual output. A destructor has a fixed no-argument special-member signature and cannot be overloaded by differing argument lists.

proper_delete.cpp releases a heap event. missing_delete.cpp intentionally loses a heap owner in an isolated noinline function so LeakSanitizer can diagnose the unreachable object and its owned arrays. mismatched_delete.cpp deliberately pairs new[] with scalar delete; both GCC's warning and the sanitizer's actual mismatch diagnostic are retained separately. The unsafe programs are never normal online demonstrations. delete destroys/deallocates a single object; delete[] handles an array and must match the allocation form.

## Logger prediction

Prediction commit precedes the first execution. The exact output is `+A +B -B copyAc use -Ac +P -P end -A `, with one final space and no newline. B dies at its block end; passing lvalue a to use(Logger) constructs Ac and destroys that parameter at call completion. P is explicitly deleted; A dies on main exit. The program and original prediction remain unchanged, and the actual output is compared character by character.
