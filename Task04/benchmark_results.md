# Actual benchmark results

Local Windows GCC14.2.0. Each configuration has five actual trials. Milliseconds include the operations described in explanation.md. Raw CSV and compiler/source/exit records are retained in outputs/.

## benchmark_O0

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| integer_paise | 2.551100, 2.593500, 3.020700, 2.552100, 2.762800 | 2.593500 |
| double_rupees | 6.179900, 6.178500, 6.104300, 6.112900, 6.054600 | 6.112900 |

Numerical result: exact integer rupees=495213.32; double rupees=495213.31999996438; observed error=-3.5623088479042053e-08 rupees.

## benchmark_O2

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| integer_paise | 0.499100, 0.557500, 0.505700, 0.502700, 0.526000 | 0.505700 |
| double_rupees | 1.511000, 1.510900, 1.498200, 1.509800, 1.498100 | 1.509800 |

Numerical result: exact integer rupees=495213.32; double rupees=495213.31999996438; observed error=-3.5623088479042053e-08 rupees.

These are measured implementation/workload results. Loop/index work, allocation, copy elision, cache locality, guard costs and scheduler noise can affect them. Big-O and observed elapsed times are reported separately.
