# Q6 tests and evidence

| ID | Scenario/input | Expected | Actual | Result |
|---|---|---|---|---|
| 1 | Empty text | size0,valid terminator,equal to empty | Checked | PASS |
| 2 | Single characters X/Y | size1,inequality | Checked | PASS |
| 3 | Hello+space+world | Hello world,length11; operands unchanged | Checked | PASS |
| 4 | Copy construct/assign then modify copy | Independent buffers/values | Checked | PASS |
| 5 | Self-assignment | Value preserved,self reference returned | Checked | PASS |
| 6 | ab+=itself | abab | Checked | PASS |
| 7 |100000-character message copy then source reassignment | Copy remains100000 chars | Checked | PASS |
| 8 | Empty operand on either side | Other value retained | Checked | PASS |
| 9 | Stream output | Exact Hello world | Checked | PASS |
| 10 | nullptr input | invalid_argument | Checked | PASS |
| 11 | Copy-plus-equals alternative | Same chain content | Checked | PASS |
| 12 | Instrumented chain,normal/no-elide builds | Correct content; actual counters recorded without assumed counts | Both builds executed | PASS |
| 13 |100000 concatenations,3 modes,5 trials,2 flags | Same checksum and sampled full content |30 actual trial checks | PASS |
| 14 | Correct owner/copy experiments under ASan/LSan | Exit0,no findings | Ubuntu evidence pending | PENDING |

Local normal compilation alone is not labelled sanitizer evidence. Raw outputs and actual copy/allocation observations are retained in outputs/.
