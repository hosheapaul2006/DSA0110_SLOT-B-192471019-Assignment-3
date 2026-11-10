# Actual benchmark results

Local Windows GCC14.2.0. Each configuration has five actual trials. Milliseconds include the operations described in explanation.md. Raw CSV and compiler/source/exit records are retained in outputs/.

## benchmark_O0

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| integer_paise | 1.038100, 1.035300, 1.031800, 1.034100, 1.031500 | 1.034100 |
| double_rupees | 2.477500, 2.476900, 2.489300, 2.477900, 2.501600 | 2.477900 |

Numerical result: exact integer rupees=495213.32000000001; double rupees=495213.31999996438; observed error=-3.5623088479042053e-08 rupees.

## benchmark_O2

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| integer_paise | 0.206200, 0.206200, 0.206200, 0.206400, 0.206400 | 0.206200 |
| double_rupees | 0.625400, 0.618500, 0.620900, 0.618700, 0.619000 | 0.619000 |

Numerical result: exact integer rupees=495213.32000000001; double rupees=495213.31999996438; observed error=-3.5623088479042053e-08 rupees.

These are measured implementation/workload results. Loop/index work, allocation, copy elision, cache locality, guard costs and scheduler noise can affect them. Big-O and observed elapsed times are reported separately.
