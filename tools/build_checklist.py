"""Trace each PDF subtask to its source and genuinely collected evidence."""
from pathlib import Path
import re
ROOT=Path(__file__).resolve().parents[1]
DATA='''Q1.01|Default, full and title-only overloaded constructors|Task01/book.hpp; Task01/outputs/main_actual.txt
Q1.02|Member initializer lists in every constructor|Task01/book.hpp
Q1.03|Delegation and single validation path|Task01/book.hpp; Task01/explanation.md
Q1.04|Negative and overlong invalid records rejected|Task01/outputs/main_stderr.txt
Q1.05|Print every constructor and draw three object states|Task01/outputs/main_actual.txt; Task01/explanation.md
Q1.06|Compiler-generated default constructor analysis|Task01/explanation.md
Q1.07|Missing default constructor array compiler error|Task01/outputs/constructor_experiments_missing_default_compiler.txt
Q1.08|Declaration order versus initializer order and real Wreorder|Task01/outputs/constructor_experiments_reorder_compiler.txt
Q1.09|Implicit conversion succeeds; explicit conversion fails|Task01/outputs/constructor_experiments_implicit_actual.txt; Task01/outputs/constructor_experiments_explicit_rejected_compiler.txt
Q1.10|Default-argument alternative, validation reuse and fixed-size complexity|Task01/explanation.md
Q2.01|Raw owning medicine array, constructor and destructor|Task02/prescription.hpp
Q2.02|Unsafe compiler-generated shallow copy and actual sanitizer report|Task02/shallow_copy.cpp; Task02/outputs/sanitizer_shallow_copy_stderr.txt
Q2.03|Deep-copy constructor and independent mutation|Task02/prescription.hpp; Task02/outputs/main_stderr.txt
Q2.04|Copy assignment, resizing and self-assignment protection|Task02/deep_copy.cpp; Task02/outputs/deep_copy_stderr.txt
Q2.05|Exception-safe ownership and Rule of Three analysis|Task02/prescription.hpp; Task02/explanation.md
Q2.06|Shallow and deep memory diagrams|Task02/explanation.md
Q2.07|Three copy-construction contexts and elision caveat|Task02/explanation.md
Q2.08|By-value versus const-reference actual copy counts/messages|Task02/outputs/main_actual.txt
Q2.09|Copy sizes 10,1000,100000 with actual timings|Task02/benchmark.cpp; Task02/benchmark_results.md
Q2.10|Correct and unsafe implementations run under ASan/LSan|Reference/sanitizer_summary.json
Q2.11|Supplied TODO1 copy constructor|Task02/complete_code.cpp; Reference/Q2_TODO_original.txt
Q2.12|Supplied TODO2 copy assignment and TODO3 release|Task02/complete_code.cpp; Reference/final_audit.json
Q2.13|Exact required Prescription output|Task02/outputs/complete_code_actual.txt; Task02/outputs/complete_code_verification.json
Q2.14|Remove only copy constructor and capture actual sanitizer error|Task02/without_copy_constructor.cpp; Task02/outputs/sanitizer_without_copy_constructor_stderr.txt
Q2.15|Vector Rule-of-Zero alternative, ownership and complexity comparison|Task02/main.cpp; Task02/explanation.md
Q3.01|Dynamic two-dimensional map allocated with new|Task03/auditorium.hpp
Q3.02|Destructor releases every allocation and prints closing summary|Task03/outputs/main_actual.txt; Task03/auditorium.hpp
Q3.03|Stack, nested block and heap new/delete lifetime demonstrations|Task03/main.cpp; Task03/outputs/main_actual.txt
Q3.04|Book/cancel seats, duplicate actions and successful statistics|Task03/outputs/main_stderr.txt
Q3.05|Prevent accidental owning copies and handle constructor failure|Task03/auditorium.hpp; Task03/explanation.md
Q3.06|Missing delete genuine leak experiment and original no-report history|Task03/outputs/sanitizer_destructor_experiments_missing_delete_stderr.txt; Reference/sanitizer_experiment_notes.md
Q3.07|Proper delete clean sanitizer execution|Task03/outputs/sanitizer_destructor_experiments_proper_delete_verification.json
Q3.08|delete versus delete[], actual mismatch warning/report|Task03/outputs/destructor_experiments_mismatched_delete_compiler.txt; Task03/outputs/sanitizer_destructor_experiments_mismatched_delete_stderr.txt
Q3.09|Destructor signature, order and RAII explanation|Task03/explanation.md
Q3.10|Seat-map memory diagram|Task03/explanation.md
Q3.11|Actual 10x20,100x200,1000x2000 allocations and process memory|Task03/memory.cpp; Task03/outputs/memory_actual.txt
Q3.12|Zero rows and invalid seat/dimension handling|Task03/outputs/main_stderr.txt
Q3.13|Exact Logger prediction saved/committed before execution|Task03/prediction_before_execution.md; Task03/outputs/predict_output_verification.json
Q3.14|Logger constructor/copy/destructor output comparison and explanation|Task03/outputs/predict_output_actual.txt; Task03/explanation.md
Q3.15|Contiguous block/vector rows alternatives, access/allocation/space analysis|Task03/explanation.md
Q4.01|Money owns signed long long paise|Task04/money.hpp
Q4.02|Arithmetic +,-,+= and comparisons ==,<|Task04/money.hpp; Task04/outputs/main_stderr.txt
Q4.03|Stream << and >>, rupee/two-digit paise formatting|Task04/money.hpp; Task04/outputs/main_actual.txt
Q4.04|Operator-based bill and named-function readability comparison|Task04/main.cpp; Task04/explanation.md
Q4.05|Value versus reference returns, member/friend and stream justification|Task04/explanation.md
Q4.06|Operator call translation and symmetric implicit-conversion analysis|Task04/explanation.md
Q4.07|0.75+0.50 carry test|Task04/outputs/main_stderr.txt
Q4.08|Negative subtraction/balance policy and comparisons|Task04/outputs/main_actual.txt
Q4.09|Input errors, signed limits and overflow protection|Task04/outputs/main_stderr.txt
Q4.10|One million integer/double additions and actual numerical error|Task04/benchmark_results.md; Task04/outputs/benchmark_O2_actual.txt
Q4.11|Money-times-Money design decision and representation limits|Task04/explanation.md
Q4.12|Supplied TODO1 member+ and TODO2 equality/friend insertion preserved|Task04/complete_code.cpp; Reference/final_audit.json
Q4.13|Exact supplied Rs200.00 and true output|Task04/outputs/complete_code_actual.txt
Q4.14|Member/friend/named alternatives and O(1) arithmetic analysis|Task04/explanation.md
Q5.01|Prefix/postfix Time increments and return semantics|Task05/playlist.hpp; Task05/outputs/main_stderr.txt
Q5.02|Time equality and ordering comparisons|Task05/outputs/main_stderr.txt
Q5.03|Const and nonconst checked Playlist subscripts|Task05/playlist.hpp; Task05/main.cpp
Q5.04|Short playback simulation|Task05/outputs/main_actual.txt
Q5.05|a=t++ and b=++t observed values|Task05/outputs/main_actual.txt
Q5.06|Minute and hour rollover including 00:59:59 to01:00:00|Task05/outputs/main_stderr.txt
Q5.07|Negative,size-equal and empty-playlist indices|Task05/outputs/main_stderr.txt
Q5.08|Dummy int,copy cost and prefix-reference explanation|Task05/explanation.md
Q5.09|Ten million prefix/postfix operations at-O0/-O2|Task05/benchmark.cpp; Task05/benchmark_results.md
Q5.10|Exact Counter prediction saved/committed before execution|Task05/prediction_before_execution.md; Task05/outputs/predict_output_verification.json
Q5.11|Evidence-based ++i recommendation without universal speed claim|Task05/explanation.md
Q5.12|Ten million checked/at/unchecked valid accesses and measured overhead|Task05/access_benchmark.cpp; Task05/benchmark_results.md
Q5.13|Throwing caller-reported errors versus PDF printing policy|Task05/explanation.md
Q5.14|O(1) access/increment and playlist storage analysis|Task05/explanation.md
Q6.01|Owning dynamically allocated null-terminated Text buffer|Task06/text.hpp
Q6.02|Constructor,copy constructor,assignment and destructor|Task06/text.hpp
Q6.03|Self-assignment and ownership exception safety|Task06/outputs/main_stderr.txt; Task06/explanation.md
Q6.04|Concatenation,==,!= and stream insertion|Task06/main.cpp; Task06/outputs/main_stderr.txt
Q6.05|Empty,single-character,long and chained messages|Task06/outputs/main_stderr.txt
Q6.06|Actual a+b+c copy/allocation instrumentation|Task06/outputs/copy_experiment_actual.txt
Q6.07|No-elide comparison and compiler-dependent interpretation|Task06/outputs/copy_experiment_no_elide_actual.txt; Task06/explanation.md
Q6.08|Correct Text and copy experiment under ASan/LSan|Task06/outputs/sanitizer_main_verification.json; Task06/outputs/sanitizer_copy_experiment_verification.json
Q6.09|100000 equivalent custom/std::string concatenation operations|Task06/benchmark.cpp; Task06/benchmark_results.md
Q6.10|Actual allocation accounting and benchmark limitations|Task06/outputs/benchmark_O2_actual.txt; Task06/explanation.md
Q6.11|List unoverloadable operators and explain fixed precedence/arity|Task06/explanation.md
Q6.12|User-defined operand,assignment-member and friend rules|Task06/explanation.md
Q6.13|std::string Rule-of-Zero alternative and justified custom use|Task06/explanation.md
Q6.14|Plus-equals-on-copy alternative and n1,n2,n3 copying/storage analysis|Task06/text.hpp; Task06/explanation.md
Q7.01|Stage1 required data/operations/three constructors and measurable contract|Task07/design_before_implementation.md
Q7.02|Stage2 custom Complex with +,-,*,/,==,<<|Task07/complex.hpp
Q7.03|Member/friend choices,formulas,class diagram and equality policy|Task07/explanation.md
Q7.04|Stage3 physically stated R,L,C and nonzero frequency|Task07/outputs/main_actual.txt
Q7.05|Resistor,inductor,capacitor and series RLC calculations|Task07/main.cpp; Task07/outputs/main_stderr.txt
Q7.06|Parallel RLC and requested two-branch product-over-sum formula|Task07/complex.hpp; Task07/outputs/main_stderr.txt
Q7.07|Independent hand-derived calculations and actual verification|Task07/hand_calculation.cpp; Task07/outputs/hand_calculation_actual.txt
Q7.08|Safe zero division,ideal shorts and singular cases|Task07/outputs/main_stderr.txt; Task07/outputs/verification_stderr.txt
Q7.09|Real/imaginary/negative/scaled values,tolerances and invalid physical input|Task07/verification.cpp; Task07/outputs/verification_stderr.txt
Q7.10|Stage4 constructor/operator rationale,safety,accuracy and limitations|Task07/explanation.md
Q7.11|Equivalent std::complex calculation and248 arithmetic comparisons|Task07/verification.cpp; Task07/outputs/verification_actual.txt
Q7.12|O(1) operations/two-component storage,division responsibilities and recommendation|Task07/explanation.md
G.01|C++17,actual compiler/CPU/platform and exact commands|README.md; Reference/compiler_version.txt; Reference/platform.txt
G.02|All valid sources compiled/run; expected diagnostic cases isolated|Reference/final_audit.json
G.03|Expected/actual/status test tables and meaningful boundaries|Task01/test_cases.md; Task02/test_cases.md; Task03/test_cases.md; Task04/test_cases.md; Task05/test_cases.md; Task06/test_cases.md; Task07/test_cases.md
G.04|Genuine sanitizer compiler/stdout/stderr/exit evidence with Linux label|Reference/sanitizer_summary.json; Reference/sanitizer_environment.txt
G.05|Real comparable benchmark data,flags and repeated trials|Reference/benchmark_summary.json
G.06|Genuine Programiz execution and29 source/output screenshots|Reference/final_audit.json
G.07|Immutable prediction chronology and TODO-only preservation audit|Reference/final_audit.json
G.08|Progressive actual commits,at least one meaningful commit per question|README.md
G.09|README and every program's purpose/compile/run instructions|README.md
G.10|Viva guide with two modifications per question|VIVA_PREPARATION.md
G.11|No tracked executables,toolchain archives or fabricated evidence|.gitignore; Reference/final_audit.json
G.12|Submission date left unspecified because PDF field is blank|README.md
G.13|Repository link and report cover identify exact existing repository|SUBMISSION_PREPARATION.md
G.14|Classroom report/link materials and manual steps prepared|SUBMISSION_PREPARATION.md'''
def evidence_links(text):
    return '; '.join('['+path+']('+path+')' if (ROOT/path).is_file() else path for path in text.split('; '))
lines=['# Assignment 3 requirement audit','', 'D Hoshea Paul |192471019 |DSA0110 |Slot B |CO3','', 'Derived from all15 pages of Reference/Assignment 3.pdf and the preparation brief. PASS denotes verified implementation/observation, including expected negative diagnostics; it does not mean an unsafe example is correct.','', '| ID | Requirement | Status | Evidence |','|---|---|---|---|']
for line in DATA.splitlines():
    identity,requirement,evidence=line.split('|');lines.append('| '+identity+' | '+requirement+' | PASS | '+evidence_links(evidence)+' |')
lines += [
'| G.15 | Complete Markdown/PDF report with readable diagrams/screenshots and layout QA | PENDING | Report generation and rendered-page verification follow |',
'| G.16 | All final files pushed,working tree clean and local/remote main equal | PENDING | Final publication/audit follow report QA |',
'| S.01 | Faculty acceptance of existing repository-name discrepancy | PENDING | PDF convention DSA01-192471019-Assignment3 differs from preserved existing name |',
'| S.02 | Student individually executes every task and can defend/modify it | PENDING | Automated/browser evidence does not certify the personal execution requirement |',
'| S.03 | Correct Classroom destination and final submission | PENDING | User submits personally; no submission performed |',
'', 'The PDF submission-date field is blank. No deadline or submission date has been invented. Normal console demonstrations omit PASS labels; required test tables and execution logs retain statuses. Sanitizer evidence is actual Ubuntu execution, separately labelled from local Windows and Programiz evidence.']
(ROOT/'ASSIGNMENT_CHECKLIST.md').write_text('\n'.join(lines)+'\n',encoding='utf-8',newline='\n')
print('Traceable PDF/brief requirement rows:',len(DATA.splitlines())+5)
