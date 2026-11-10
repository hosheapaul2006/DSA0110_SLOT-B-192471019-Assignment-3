# Q1 - Library book records

## Design and algorithm

Book stores a 128-byte title array, a 96-byte author array and an int copy count. Both character arrays include space for a null terminator. Every constructor has a member initializer list. The default constructor delegates to the full constructor with empty fields and zero copies. The explicit title-only constructor delegates with author "Donated" and one copy. The full constructor validates count >=0, rejects overlong text, then copies validated characters into its owned arrays. Throwing prevents a successfully constructed invalid object; no silent truncation occurs.

Pseudocode: initialize fixed fields; reject negative copies; reject title length>=128 or author length>=96; copy each field and terminate it. The independent full implementation validates once, so every delegating path shares the invariant.

## Constructor experiments

The missing-default source declares Book(int) but no Book(), so Book records[2] requires an unavailable default constructor and fails. A compiler supplies an implicit default constructor only when no user-declared constructor prevents its implicit declaration; it can still be deleted if member/base initialization is invalid. Declaring a parameterized constructor does not automatically provide a default constructor.

The reorder experiment safely initializes first=10 and second=20 with a deliberately reversed initializer list. GCC's actual -Wreorder warning and output10 20 are retained. There is no uninitialized read: members initialize in declaration order, regardless of list order. An initializer list directly initializes members; assignment in the body would first default-initialize/class-initialize them and then assign, and cannot replace required initialization of const/reference members.

The implicit experiment accepts print(3) by converting int to Book. The explicit version rejects the same implicit argument conversion. Explicit still permits direct construction Book(3). Mark single-argument constructors explicit when automatic conversions would be surprising or would hide units/meaning. The production title-only constructor is explicit.

## Three constructed object states

```text
empty:    [title=""                   ][author=""       ][copies=0]
donation: [title="Donated Algorithms" ][author="Donated"][copies=1]
complete: [title="C++ Foundations"    ][author="A. Reader"][copies=4]
Each object owns its fixed arrays; the objects share no character allocation.
```

## Alternative and evaluation

A single defaulted constructor could be `Book(string_view title="", string_view author="", int copies=0)`. Reuse the same full validation and storage logic. This creates a different title-only default (zero copies) unless an explicit policy selects one for donations. Delegating overloads make that donation policy clear at the call site. Neither design inherently requires more validation sites: both can validate in exactly one implementation.

For bounded fixed arrays, construction is O(1) time and O(1) auxiliary/per-object storage with respect to the number of books. Copying up to L supplied characters is O(L) if field capacity is treated as a variable; L is bounded here. The PDF's std::string alternative is not generally O(1) in unbounded title length: storing a string copy costs O(L) time/space. sizeof(Book) is measured in the actual local output, not assumed to be a language-wide ABI constant. Fixed storage gives predictable size but rejects longer metadata; a real library might prefer std::string.
