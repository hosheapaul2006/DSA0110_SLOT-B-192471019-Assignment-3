# Actual benchmark results

Local Windows GCC14.2.0. Each configuration has five actual trials. Milliseconds include the operations described in explanation.md. Raw CSV and compiler/source/exit records are retained in outputs/.

## benchmark_O0

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| medicines=10, copies=10000 | 1.152800, 1.163500, 1.123700, 1.121800, 1.118400 | 1.123700 |
| medicines=1000, copies=200 | 1.790400, 3.141000, 1.764100, 1.761500, 1.762900 | 1.764100 |
| medicines=100000, copies=5 | 7.718300, 7.453800, 6.689700, 6.616200, 6.737700 | 6.737700 |

Median complete copy lifetime per copy (allocation, content traversal and destruction included):
- medicines=10, copies=10000: 0.112370 microseconds/copy.
- medicines=1000, copies=200: 8.820500 microseconds/copy.
- medicines=100000, copies=5: 1347.540000 microseconds/copy.

## benchmark_O2

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| medicines=10, copies=10000 | 0.651400, 0.524900, 0.523300, 0.518200, 0.520700 | 0.523300 |
| medicines=1000, copies=200 | 0.776700, 0.763900, 0.768200, 0.766600, 0.766000 | 0.766600 |
| medicines=100000, copies=5 | 5.158300, 4.617500, 4.671300, 4.591300, 4.552200 | 4.617500 |

Median complete copy lifetime per copy (allocation, content traversal and destruction included):
- medicines=10, copies=10000: 0.052330 microseconds/copy.
- medicines=1000, copies=200: 3.833000 microseconds/copy.
- medicines=100000, copies=5: 923.500000 microseconds/copy.

These are measured implementation/workload results. Loop/index work, allocation, copy elision, cache locality, guard costs and scheduler noise can affect them. Big-O and observed elapsed times are reported separately.
