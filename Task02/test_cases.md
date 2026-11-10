# Q2 tests and evidence

| ID | Scenario/input | Expected | Actual | Result |
|---|---|---|---|---|
| 1 | Copy construct and assign prescription; mutate p | q/r keep Paracetamol, p becomes Ibuprofen | Checked | PASS |
| 2 | Change q's second medicine | p keeps ORS | Checked | PASS |
| 3 | By-value parameter mutation | Temporary changes local copy only | Checked | PASS |
| 4 | Const-reference parameter | Reads Ibuprofen; no new copy | Checked | PASS |
| 5 | Constructor instrumentation | Value1,reference0 copies | Counts observed and checked | PASS |
| 6 | p=p | Data/count preserved | Checked | PASS |
| 7 | Empty copy/assignment | size0; valid destruction | Checked | PASS |
| 8 | Negative count and empty index | Exceptions | Caught and checked | PASS |
| 9 | vector<string> copy | Independent mutation | Checked | PASS |
| 10 | Deep-copy resized assignment and returned reference | Three independent values; assignment returns self | deep_copy.cpp checked | PASS |
| 11 | Supplied TODO | Exact expected line | Character comparison matched | PASS |
| 12 | Copy sizes10,1000,100000, five trials at both flags | Every copied length checksum agrees |30 actual trial checks passed | PASS |
| 13 | Correct raw owners under ASan/LSan | Exit0,no sanitizer findings | Remote experiment not yet collected | PENDING |
| 14 | Shallow copy and removed-copy-constructor experiment | Real ASan use-after-free/double-delete diagnostic | Compiled only locally; remote run pending | PENDING |

Local outputs include actual stdout/stderr, source hashes, flags and exit codes. Sanitizer status must be updated only after real Ubuntu execution.
