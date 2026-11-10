# Actual benchmark results

Local Windows GCC14.2.0. Each configuration has five actual trials. Milliseconds include the operations described in explanation.md. Raw CSV and compiler/source/exit records are retained in outputs/.

## access_benchmark_O0

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| checked_subscript | 65.416700, 66.126500, 66.114300, 65.989800, 65.822700 | 65.989800 |
| at | 74.438900, 74.993600, 74.787900, 74.217600, 73.998800 | 74.438900 |
| unchecked | 46.525700, 46.592800, 46.567900, 46.514800, 47.030900 | 46.567900 |

## access_benchmark_O2

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| checked_subscript | 2.695000, 2.682400, 2.702900, 2.680100, 2.667100 | 2.682400 |
| at | 2.662700, 2.654500, 2.663700, 2.643300, 2.687200 | 2.662700 |
| unchecked | 2.624600, 2.608800, 2.506900, 2.582300, 2.496500 | 2.582300 |

## benchmark_O0

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| prefix_discarded | 44.388400, 44.325400, 44.166300, 44.863000, 44.189000 | 44.325400 |
| postfix_discarded | 55.094700, 55.022300, 54.849000, 54.713900, 54.820300 | 54.849000 |
| prefix_value_used | 52.734400, 52.534500, 52.473300, 52.357800, 53.116300 | 52.534500 |
| postfix_value_used | 63.856700, 63.412100, 63.474800, 63.457300, 63.288300 | 63.457300 |

## benchmark_O2

| Workload | Actual trials (ms) | Median (ms) |
|---|---|---|
| prefix_discarded | 2.976000, 2.980200, 3.010400, 2.979100, 3.005800 | 2.980200 |
| postfix_discarded | 2.974500, 2.984800, 3.005300, 3.046900, 2.982600 | 2.984800 |
| prefix_value_used | 3.483700, 3.525600, 3.528400, 3.625500, 3.502800 | 3.525600 |
| postfix_value_used | 3.526900, 3.530000, 3.664600, 3.715900, 3.623500 | 3.623500 |

These are measured implementation/workload results. Loop/index work, allocation, copy elision, cache locality, guard costs and scheduler noise can affect them. Big-O and observed elapsed times are reported separately.
