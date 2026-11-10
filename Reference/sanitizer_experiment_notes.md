# Sanitizer experiment history

Run1: commit57584a3, GitHub Actions run42847619277. Seven correct programs
exited0 with empty sanitizer stderr. Both Q2 shallow-copy variants and Q3's
new[]/delete mismatch produced the expected actual ASan error. The missing-delete
experiment exited0 without a leak report, so its expected-diagnostic check FAILED.
All original records/bytes are retained in Reference/sanitizer_attempt1/.

The absence of a leak diagnostic was not labelled proof of correct ownership:
the source clearly omitted delete. Leak detection conservatively treats pointers
found in live registers/stacks as roots. The revised isolated experiment creates
the leaked event in a worker thread and joins it before process exit, removing
that completed thread's stack/register roots. It still omits delete and does not
suppress errors or alter the correct owning class. The later real run determines
whether the expected diagnostic is actually observed; do not assume its result.
