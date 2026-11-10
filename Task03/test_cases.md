# Q3 tests and evidence

| ID | Scenario/input | Expected | Actual | Result |
|---|---|---|---|---|
| 1 | Book first/last valid seat | Both succeed | Checked | PASS |
| 2 | Duplicate booking | false,statistics unchanged | Checked | PASS |
| 3 | Cancel then repeat cancellation | true then false | Checked | PASS |
| 4 | Remaining occupied count |1 | Checked | PASS |
| 5 | Nested and heap events | Booking succeeds; each closing message at correct lifetime end | Checked and output retained | PASS |
| 6 | Zero rows | No allocations,zero occupancy; booking throws | Checked | PASS |
| 7 | Negative dimensions/indices | Exceptions | Checked | PASS |
| 8 | Copy traits | Copy construction/assignment disabled | Checked | PASS |
| 9 | Logger | Exact precommitted output including final space/no newline | Matched | PASS |
| 10 | Three required seat-map sizes | Actual requested payload,pointer bytes and allocation counts; all cells touched | memory_actual.txt; payloads200/20000/2000000 bytes plus80/800/8000 pointer bytes | PASS |
| 11 | Proper heap delete | Open/close messages | Executed locally and clean under Ubuntu ASan/LSan | PASS |
| 12 | Missing delete and new[]/delete mismatch | Actual leak/mismatch sanitizer reports | Actual102-byte/four-allocation leak and alloc-dealloc-mismatch reports | PASS |

Process resident memory is measured separately from requested allocation bytes. Sanitizer statuses reflect the actual successful remote experiment; the first no-report leak attempt is preserved separately.
