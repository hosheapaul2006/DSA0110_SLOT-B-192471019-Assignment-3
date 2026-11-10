# Q5 executed tests

| ID | Scenario/input | Expected | Actual | Result |
|---|---|---|---|---|
| 1 | t=00:59:58,a=t++,b=++t | a00:59:58,b/t01:00:00 | Checked and printed | PASS |
| 2 | Identical/earlier/later comparisons | Correct equality and ordering | Checked | PASS |
| 3 |00:00:59 increment |00:01:00 | Checked | PASS |
| 4 | Hour boundary formatting |01:00:00 | Exact string matched | PASS |
| 5 | Edit playlist[0],read const playlist | Edited title visible; const reference returned | Values/type checked | PASS |
| 6 | playlist[-1],playlist[size] | out_of_range,caller prints message | Both caught and checked | PASS |
| 7 | Empty playlist index0 | out_of_range | Checked | PASS |
| 8 | Negative Time field | invalid_argument | Checked | PASS |
| 9 | Maximum total seconds increment | overflow_error,state unchanged | Checked | PASS |
| 10 | Supplied Counter | Exact5 7 7 newline | Precommitted prediction matched | PASS |
| 11 |10M increments,4 modes,5 trials,2 flags | Every timer and old-value checksum correct |40 trial checks | PASS |
| 12 |10M accesses,3 modes,5 trials,2 flags | Identical expected string-length checksum; valid indices only |30 trial checks | PASS |

Compiler/source/command records and actual local results are in outputs/. Programiz evidence is separate from flagged local benchmarks.
