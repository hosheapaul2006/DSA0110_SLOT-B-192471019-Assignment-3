# Actual benchmark results

Local Windows GCC14.2.0. Each configuration has five actual trials. Milliseconds include the operations described in explanation.md. Raw CSV and compiler/source/exit records are retained in outputs/.

## benchmark_O0

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| custom_direct | 6.714300, 6.727100, 6.775300, 6.783600, 6.804500 | 6.775300 |
| std_string | 7.553100, 7.489400, 7.585400, 7.650600, 7.671500 | 7.585400 |
| custom_copy_plus | 14.670700, 14.811700, 14.831700, 14.891200, 14.973400 | 14.831700 |

Trial1 custom allocation counts: custom_direct=200000, std_string=not_instrumented, custom_copy_plus=500000. std::string allocation counts are not instrumented.

## benchmark_O2

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| custom_direct | 5.601800, 5.587200, 5.593400, 5.696300, 5.589800 | 5.593400 |
| std_string | 6.038400, 6.097800, 6.129900, 6.133500, 6.125200 | 6.125200 |
| custom_copy_plus | 12.299900, 12.434700, 12.596200, 12.705200, 12.645400 | 12.596200 |

Trial1 custom allocation counts: custom_direct=200000, std_string=not_instrumented, custom_copy_plus=500000. std::string allocation counts are not instrumented.

These are measured implementation/workload results. Loop/index work, allocation, copy elision, cache locality, guard costs and scheduler noise can affect them. Big-O and observed elapsed times are reported separately.
