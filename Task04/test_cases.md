# Q4 executed tests

| ID | Scenario/input | Expected | Actual | Result |
|---|---|---|---|---|
| 1 |120.75+79.25-20 | Rs180.00; named add yields200 | Checked and printed | PASS |
| 2 |0.75+0.50 | Rs1.25 | Checked and printed | PASS |
| 3 |0.50-0.75 | Rs -0.25; less than zero | Checked | PASS |
| 4 | += | Updated value and reference to self | Checked | PASS |
| 5 | Minimum long long | Rs -92233720368547758.08 without abs overflow | Exact string checked | PASS |
| 6 | Input -12.3 | -1230 paise | Checked | PASS |
| 7 |1.234,abc,12.,+,positive overflow input | failbit; target remains Rs7 | Five cases checked | PASS |
| 8 | Minimum signed input | LLONG_MIN paise | Checked | PASS |
| 9 | Max+1 and min-1 | overflow_error; += target unchanged | Checked | PASS |
| 10 | Stream with custom fill | Rs2.05 then **1 | Exact string checked | PASS |
| 11 | Supplied TODO | Rs200.00 newline true newline | Exact characters matched | PASS |
| 12 | Million integer/double additions,five trials,both flags | Exact integer checksum; bounded observed floating error |20 actual checks | PASS |

Actual outputs, compiler records and check logs are in outputs/. Benchmark results are observations, not universal speed guarantees.
