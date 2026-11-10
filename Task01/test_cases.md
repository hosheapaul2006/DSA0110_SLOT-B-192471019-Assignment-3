# Q1 executed tests

| ID | Scenario/input | Expected | Actual | Result |
|---|---|---|---|---|
| 1 | Default Book | Empty title,0 copies | C++ check and printed state match | PASS |
| 2 | Title-only donation | Donated Algorithms,1 copy | Check/printed state match | PASS |
| 3 | Full Book | C++ Foundations,A. Reader,4 | Check/printed state match | PASS |
| 4 | Array of2 Books | Both default to0 copies | Checked | PASS |
| 5 | Negative copies | invalid_argument; no valid object produced | Exception caught and checked | PASS |
| 6 | Zero copies | Accepted | Checked | PASS |
| 7 | Title127/author95 characters | Accepted without truncation | Checked | PASS |
| 8 | Title128/author96 characters | length_error | Both exceptions checked | PASS |
| 9 | No Book() and default array | No matching constructor error | Actual GCC diagnostic captured | PASS |
| 10 | Reversed initializer list | -Wreorder; declaration-order values10 20 | Warning and output observed | PASS |
| 11 | Nonexplicit int constructor | print(3) compiles,prints Copies=3 | Observed | PASS |
| 12 | Explicit int constructor | Implicit argument conversion rejected | Actual compiler error captured | PASS |

The negative compiler programs are not executable deliverables. All observations have matching files in outputs/. Programiz evidence is recorded separately after online execution.
