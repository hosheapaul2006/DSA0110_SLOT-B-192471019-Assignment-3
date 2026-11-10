# DSA0110 - Assignment 3 (CO3)

**Student:** D Hoshea Paul  
**Register number:** 192471019  
**Slot:** B  
**Course outcome:** Constructors, destructors and operator overloading  
**Language:** C++17 for every assignment implementation and benchmark

Repository: [Assignment 3](https://github.com/hosheapaul2006/DSA0110_SLOT-B-192471019-Assignment-3).
Specification: [original 15-page PDF](Reference/Assignment%203.pdf).
Report: [PDF](Report/Assignment3_Report.pdf) | [Markdown](Report/Assignment3_Report.md).
Also see [requirement audit](ASSIGNMENT_CHECKLIST.md), [viva preparation](VIVA_PREPARATION.md)
and [personal submission steps](SUBMISSION_PREPARATION.md).

The PDF's submission-date field is blank. No deadline or submission date is invented.
Its repository convention is `DSA01-192471019-Assignment3`; the existing name is
preserved as instructed and the discrepancy is flagged for faculty acceptance.

## Environments and evidence

- Local normal tests/benchmarks: Windows, GCC14.2.0 (WinLibs MinGW-w64 x86_64 UCRT POSIX SEH).
- Actual local version, CPU and platform: Reference/compiler_version.txt, processor.txt and platform.txt.
- Sanitizers: GCC13.3.0 on GitHub-hosted Ubuntu24.04, with actual version/platform/commit in Reference/sanitizer_environment.txt.
- Online demonstrations: actual [Programiz C++ compiler](https://www.programiz.com/cpp-programming/online-compiler/) runs and source/output screenshots.

The local Windows GCC distribution lacks ASan/LeakSanitizer runtime libraries.
The approved Ubuntu workflow provides genuine memory-safety evidence instead;
it is explicitly remote Linux evidence and is not described as local execution.
[Successful sanitizer run](https://github.com/hosheapaul2006/DSA0110_SLOT-B-192471019-Assignment-3/actions/runs/42848528926).
Raw per-program compiler/stdout/stderr/exit records are under the corresponding outputs/ folders.

28 base programs compiled successfully. Of these,24 safe normal programs ran locally;
four deliberately unsafe programs ran only under isolated Linux sanitizers. Eleven
self-contained browser-source builds give39 successfully compiled source files.
Two additional sources intentionally fail compilation. There are41 successful
normal local build/run configurations, including optimization/elision variants,
and838 recorded local check evaluations (503 in base configurations,335 repeated
in derived browser builds). Eleven sanitizer cases comprise seven clean cases
and four expected-diagnostic cases. Fifteen Programiz programs matched local
stdout;29 genuine screenshots include additional source sections for long files.
These counts include repeated configurations, not838 distinct input scenarios.

## Compile and execute

From the repository root with a working g++ on PATH:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
g++ --version
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 Task01/main.cpp -o build/Q1.exe
.\build\Q1.exe
.\build\Q1.exe --tests
```

The normal run prints coursework results. The optional --tests argument adds
actual check labels/counts on stderr, leaving Programiz console demonstrations
clear of PASS labels. Test tables include expected/actual/status as required.
Inputs are reproducible C++ fixtures embedded in each main; no stdin input file
is needed. Money stream tests use explicit istringstream tokens in Task04/main.cpp.

The exact compiler used here can be selected in a PowerShell session:

```powershell
$cxx = 'C:\Projects\SLOT B\.tools\mingw64\bin\g++.exe'
$env:PATH = (Split-Path $cxx) + ';' + $env:PATH
& $cxx -std=c++17 -Wall -Wextra -Wpedantic -O0 Task07/main.cpp -o build/Q7.exe
.\build\Q7.exe
```

The toolchain/executables are not committed. The PATH change affects only the
current shell. A fresh clone can use another working C++17 g++ installation.

## Independent program index

Compile each source separately with the standard command above and a unique
output executable name. Quoted local headers are found relative to the source.
Every actual compilation command is also in outputs/*_verification.json.

| Task | Source | Purpose / execution policy |
|---|---|---|
| Q1 | main.cpp | Three Book states and invariant tests |
| Q1 | constructor_experiments/implicit.cpp | Successful nonexplicit conversion |
| Q1 | constructor_experiments/reorder.cpp | Expected -Wreorder warning; safe printed values |
| Q1 | constructor_experiments/missing_default.cpp | Expected compile error; no default constructor |
| Q1 | constructor_experiments/explicit_rejected.cpp | Expected compile error; implicit conversion rejected |
| Q2 | main.cpp | Deep copies, copy instrumentation, self-assignment, vector alternative |
| Q2 | deep_copy.cpp | Independent copy/assignment ownership demonstration |
| Q2 | complete_code.cpp | Exact supplied TODO exercise |
| Q2 | shallow_copy.cpp | Unsafe: compile locally, execute only with sanitizer |
| Q2 | without_copy_constructor.cpp | Unsafe supplied-program experiment: sanitizer only |
| Q2 | benchmark.cpp | Copy sizes10,1000,100000; five trials per flag |
| Q3 | main.cpp | Stack/block/heap map lifetimes and booking tests |
| Q3 | predict_output.cpp | Exact supplied Logger prediction program |
| Q3 | memory.cpp | Requested map bytes plus actual process resident measurement |
| Q3 | destructor_experiments/proper_delete.cpp | Correct explicit heap release |
| Q3 | destructor_experiments/missing_delete.cpp | Unsafe isolated leak: sanitizer only |
| Q3 | destructor_experiments/mismatched_delete.cpp | Unsafe new[]/delete mismatch: sanitizer only |
| Q4 | main.cpp | Money arithmetic, formatting, input and boundaries |
| Q4 | complete_code.cpp | Exact supplied Money TODO exercise |
| Q4 | benchmark.cpp | One million integer/double additions |
| Q5 | main.cpp | Time/playlist simulation, const access and invalid indices |
| Q5 | predict_output.cpp | Exact supplied Counter prediction program |
| Q5 | benchmark.cpp | Ten million prefix/postfix operations |
| Q5 | access_benchmark.cpp | Ten million checked/at/unchecked valid accesses |
| Q6 | main.cpp | Owning Text, copies, long/empty values and chaining |
| Q6 | copy_experiment.cpp | Actual constructor/allocation counts; optional no-elide variant |
| Q6 | benchmark.cpp |100000 equivalent custom/std::string/+=-on-copy chains |
| Q7 | hand_calculation.cpp | Independent scalar evaluation of hand-derived equations |
| Q7 | main.cpp | Custom Complex R/L/C/series/parallel calculator |
| Q7 | verification.cpp |248 arithmetic comparisons plus numerical/circuit boundaries |

Each task also has online.cpp, a mechanically flattened copy of its header(s)
and main for Programiz's single-file editor. Additional online_* files flatten
the listed deep-copy, proper-delete, copy-count and Complex verification examples.
They are compiled locally before pasting exactly that saved source into Programiz.
They are derived browser builds, not different assignment designs.

## Diagnostics, benchmarks and sanitizer commands

Expected constructor diagnostics:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Wreorder -O0 Task01/constructor_experiments/reorder.cpp -o build/reorder.exe
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 Task01/constructor_experiments/missing_default.cpp -o build/missing.exe
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 Task01/constructor_experiments/explicit_rejected.cpp -o build/explicit.exe
```

The last two commands should fail. This is expected compiler evidence, not an
unfixed solution failure. The reorder program remains safe to execute.

For every benchmark source in the index, build/run separately with both flags:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 Task05/benchmark.cpp -o build/Q5_O0.exe
.\build\Q5_O0.exe --tests
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 Task05/benchmark.cpp -o build/Q5_O2.exe
.\build\Q5_O2.exe --tests
g++ -std=c++17 -Wall -Wextra -Wpedantic -O0 -fno-elide-constructors Task06/copy_experiment.cpp -o build/copies_no_elide.exe
.\build\copies_no_elide.exe --tests
```

On a supported Linux GCC installation, the actual sanitizer pattern is:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O1 -fsanitize=address -fno-omit-frame-pointer Task02/main.cpp -o main_asan
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1:alloc_dealloc_mismatch=1 ./main_asan
python3 tools/sanitizers.py
```

The workflow checks out the exact public commit and runs the same sanitizer
script. It exports raw evidence losslessly into its job log. Correct programs
must exit0 with empty sanitizer stderr; unsafe cases must exit nonzero with the
specific expected ASan/LSan report. A PASS on an unsafe test means the defect was
genuinely diagnosed, never that the unsafe implementation is correct.

The first missing-delete test produced no leak diagnostic; that failed expectation
and all its original bytes are preserved in Reference/sanitizer_attempt1/.
The revised completed-worker-thread test produced102 leaked bytes/four allocations.
See Reference/sanitizer_experiment_notes.md for the investigation and actual run links.

## Reproduction tools and results

All solutions/tests/benchmarks are C++17. Python only orchestrates compilation,
evidence auditing, summaries and document rendering. Optional reproduction:

```powershell
$env:CXX = 'C:\Projects\SLOT B\.tools\mingw64\bin\g++.exe'
python tools/run_all.py
python tools/summarize.py
python tools/audit.py
```

Rerunning changes measured output files and may dirty the working tree; predictions
are never rewritten. Recompute summaries/report when publishing a fresh dataset.
PDF regeneration additionally uses reportlab,pypdf,Pillow with tools/build_report.py.

| Question | Verified outcome |
|---|---|
| Q1 | Constructor/invariant cases; actual missing-default, reorder and explicit diagnostics |
| Q2 | Independent owners; exact TODO output; clean and expected-failure sanitizer evidence |
| Q3 | Correct lifetimes/bookings; exact Logger prediction; actual memory/leak/mismatch evidence |
| Q4 | Integer arithmetic/streams/carry/negative/limits; exact TODO output and million-addition results |
| Q5 | Prefix/postfix/const access/range/rollover; exact Counter prediction and measured access/increment costs |
| Q6 | Rule of Three, empty/long/chained values; actual elision counts, clean sanitizer runs and measured alternatives |
| Q7 | Four design stages; scalar reference,248 standard-library arithmetic comparisons and singular/boundary cases |

At-O2, median copy lifetimes for10/1000/100000 medicines were0.05233/3.833/923.5
microseconds per complete copy lifetime. Discarded prefix/postfix medians were
2.9802/2.9848ms for10M operations, showing no meaningful universal postfix penalty.
Checked/unchecked access medians were2.6824/2.5823ms for10M accesses. Custom direct
Text/std::string medians were5.5934/6.1252ms for100000 chains on this workload.
All flags, five-trial data, numerical errors and methodology limits are in the
task benchmark_results.md files; these observations do not guarantee other platforms.

## Contents and remaining student steps

```text
Reference/       Original PDF/source extracts, environment, sanitizer history/audit
Task01..Task07/  C++ sources/headers, explanation.md, test_cases.md
  outputs/      Original predictions, actual outputs, diagnostics, timings and JSON
  screenshots/  Genuine Programiz source/output views
Report/         Assignment3_Report.md and Assignment3_Report.pdf
tools/          Build, audit, summary, sanitizer and document orchestration
.github/        Ubuntu sanitizer workflow
```

Predictions were committed before execution: Q3 commit0aa0309 and Q5 commita7c3793.
The original files and supplied programs remain unchanged after observation.
Normal valid sources compile without unexpected warnings. Original TODO code
outside the replaced sections is verified against the extracted PDF programs.

The user will submit Classroom personally. Review the report, reproduce individual
executions required by the PDF, and verify the correct course/Assignment3 destination.
Faculty acceptance of the existing repository-name discrepancy remains pending.
The final handoff verifies that main is pushed and the working tree is clean.
