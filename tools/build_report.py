"""Assemble the academic report from implemented sources and actual evidence."""
import json
from pathlib import Path
import re
from report_layout import ROOT,REPORT,URL,render_pdf

def read(path):return (ROOT/path).read_text(encoding='utf-8-sig')
def block(path,language='cpp',heading=True):
    return ('### '+path+'\n\n' if heading else '')+'```'+language+'\n'+read(path).rstrip('\n')+'\n```\n\n'
def body(path):return read(path).split('\n',1)[1].lstrip()+'\n\n'
def excerpt(path,maximum=18):
    lines=read(path).splitlines();result='Raw diagnostic excerpt from '+path+' (full unchanged report retained in the repository):\n\n```text\n'+'\n'.join(lines[:maximum])+'\n```\n\n'
    summary=[line for line in lines if 'SUMMARY:' in line]
    if summary:result+='Actual diagnostic summary:\n\n```text\n'+'\n'.join(summary)+'\n```\n\n'
    return result

TITLES={1:'Constructors and Constructor Overloading',2:'Copy Constructors: Shallow and Deep Copies',3:'Dynamic Constructors and Destructors',4:'Arithmetic and Stream Operator Overloading',5:'Increment, Comparison and Subscript Operators',6:'Rules of Operator Overloading',7:'Complex-Number Class from First Principles'}
CONTEXT={1:'Library Book Records: construct empty, title-only donated and full records, enforce valid copy counts and investigate constructor behaviour. PDF pages2-3;6 marks.',
2:'Hospital Prescription Records: diagnose a raw owning shallow copy, implement independent deep copying, complete the supplied program, observe copy counts and measure copying. PDF pages3-6;7 marks.',
3:'Auditorium Seat Booking: allocate a dynamic two-dimensional map, book/cancel seats, summarize/release ownership on destruction and investigate object lifetimes/memory. PDF pages6-8;7 marks.',
4:'Billing Counter: represent Money in integer paise, overload arithmetic/comparison/streams, investigate precision and complete the supplied operators. PDF pages8-10;7 marks.',
5:'Music Playlist and Track Timer: implement prefix/postfix increments, comparisons, checked const/nonconst subscripts and a playback simulation; verify predictions and measured costs. PDF pages11-13;6 marks.',
6:'SMS Gateway Text: implement an owning character array, Rule-of-Three copying, operators, actual copy/elision investigation and std::string comparison. PDF pages13-14;7 marks.',
7:'AC Circuit Impedance Calculator: interpret, design, implement/verify and defend a custom Complex class without substituting std::complex for its implementation. PDF pages14-15;10 marks.'}
HEADERS={1:'book.hpp',2:'prescription.hpp',3:'auditorium.hpp',4:'money.hpp',5:'playlist.hpp',6:'text.hpp',7:'complex.hpp'}

def make_markdown():
    audit=json.loads(read('Reference/final_audit.json'));sanitizers=json.loads(read('Reference/sanitizer_summary.json'))
    parts=['# DSA0110 - Assignment 3 (CO3)\n\nName: D Hoshea Paul\n\nRegister Number: 192471019\n\nSlot: B\n\nGitHub Repository: '+URL+'\n\nSubmission date: not specified in the assignment PDF.\n\n',
    '## Scope, environment and evidence\n\nThe original 15-page Assignment 3 (CO3) PDF contains Q1-Q7, total 50 marks. All implementations and benchmarks use C++17. The required process is Design -> Implementation -> Analysis -> Evaluation. This report is supported by the complete sources, exact commands and unchanged raw evidence in the repository.\n\n',
    'Local compiler: GCC 14.2.0,WinLibs MinGW-w64 x86_64 UCRT POSIX SEH. Normal flags: `-std=c++17 -Wall -Wextra -Wpedantic -O0`; benchmarks additionally compile separately at `-O2`. Actual local platform/CPU:\n\n```text\n'+read('Reference/platform.txt').strip()+'\n'+read('Reference/processor.txt').strip()+'\n```\n\n',
    'The local Windows compiler lacks ASan/LeakSanitizer runtime libraries. With user approval, memory experiments ran on GitHub-hosted Ubuntu 24.04 with actual GCC 13.3.0. These are remote Linux observations, distinct from local Windows and Programiz evidence. Actual sanitizer flags: `-std=c++17 -Wall -Wextra -Wpedantic -g -O1 -fsanitize=address -fno-omit-frame-pointer`, with `ASAN_OPTIONS=detect_leaks=1:halt_on_error=1:alloc_dealloc_mismatch=1`.\n\n',
    f"Recorded execution: {audit['base_successfully_compiled_programs']} successfully compiled base programs; 11 additional self-contained browser builds; {audit['compiled_source_files_including_online']} successfully compiled source files total. 24 safe base programs ran locally and four unsafe base programs ran only under isolated sanitizers. Two sources deliberately fail compilation. There are {audit['normal_build_run_configurations']} successful normal local build/run configurations, including optimization/elision variants, {audit['recorded_local_check_evaluations']} recorded local check evaluations ({audit['base_check_evaluations']} base plus 335 repeated derived-build checks), 11 sanitizer cases, 15 Programiz program comparisons and 29 genuine screenshots. Check counts include repeated configurations, not 838 distinct input scenarios.\n\n",
    'Console demonstrations show coursework output; --tests sends verification labels/counts to stderr only in local testing. Python files orchestrate builds/auditing/documents and do not implement assignment solutions. online.cpp/online_* are mechanically flattened source files made from the same headers and drivers, compiled locally before those exact bytes are pasted into Programiz. The supplied prediction/TODO programs retain their original structure.\n\n',
    '### Prediction chronology\n\n| Question | Pre-execution commit | Exact result |\n|---|---|---|\n| Q3 Logger |0aa0309 | Matched, including final space and no newline |\n| Q5 Counter |a7c3793 | Matched 5 7 7 plus newline |\n\nThe original predictions are immutable. Windows CRLF is decoded as LF for character comparisons; no other whitespace is discarded. Programiz result-pane text is checked separately against all expected stdout characters, allowing only its trailing UI blank space/success banner. Q7\'s hand-derived scalar reference/design was also saved before the custom Complex implementation.\n\n',
    '### Submission scope\n\nThe existing repository name is preserved. The PDF convention DSA01-192471019-Assignment3 differs from it and requires faculty acceptance. The PDF date field is blank; no deadline/submission date has been invented. The user will submit Classroom personally. The PDF also requires individual student execution: automated/browser runs establish the recorded executions, not the student\'s personal completion of that requirement.\n\n']
    figure=0
    for number in range(1,8):
        folder=f'Task{number:02}'
        parts += [f'# Q{number} - {TITLES[number]}\n\n','## Problem context and objective\n\n'+CONTEXT[number]+'\n\n',body(folder+'/explanation.md'),
                  '## Relevant C++17 implementation\n\n',block(folder+'/'+HEADERS[number]),block(folder+'/main.cpp')]
        if number==1:
            parts+=['## Constructor experiments: actual compiler evidence\n\n']
            for name in ['missing_default','reorder','implicit','explicit_rejected']:
                parts += [block(folder+'/constructor_experiments/'+name+'.cpp')]
                compiler=folder+'/outputs/constructor_experiments_'+name+'_compiler.txt'
                if read(compiler).strip():parts += [block(compiler,'text')]
                actual=folder+'/outputs/constructor_experiments_'+name+'_actual.txt'
                if (ROOT/actual).exists():parts += [block(actual,'text')]
        if number in [2,4]:
            parts += ['## Completed supplied TODO program\n\nOnly the requested TODO regions were replaced. Reference contains the original PDF-extracted source, and the preservation audit reconstructs the completed program from those replacements.\n\n',block(folder+'/complete_code.cpp'),'Required expected output:\n\n',block(folder+'/outputs/complete_expected.txt','text'),'Actual execution output:\n\n',block(folder+'/outputs/complete_code_actual.txt','text')]
        if number in [3,5]:
            parts += ['## Exact pre-execution prediction and observation\n\n',body(folder+'/prediction_before_execution.md'),block(folder+'/predict_output.cpp'),'Original predicted output (Q3 ends with a space and no newline):\n\n',block(folder+'/outputs/predicted_output.txt','text'),'Actual output:\n\n',block(folder+'/outputs/predict_output_actual.txt','text')]
        if number==3:
            parts += ['## Memory measurement evidence\n\n',block(folder+'/memory.cpp'),block(folder+'/outputs/memory_actual.txt','text')]
        if number in [2,3,6]:
            parts += ['## Authentic sanitizer evidence\n\nSource SHA256,compiler command,run command,ASAN_OPTIONS,exit code,UTC time and workflow URL accompany each raw log. PASS below means the expected observation occurred; a negative example remains intentionally defective.\n\n','| Source | Expected | Actual exit | Observation |\n|---|---|---|---|\n']
            for record in sanitizers:
                if not record['source'].startswith(folder+'/'):continue
                negative=bool(record['expected_error_pattern'])
                parts += ['| '+record['source']+' | '+('Defect diagnostic' if negative else 'No sanitizer findings')+' | '+str(record['run_exit'])+' | '+('Expected real report observed' if negative else 'Exit 0, empty sanitizer stderr')+' |\n']
            parts += ['\nActual successful job: https://github.com/hosheapaul2006/DSA0110_SLOT-B-192471019-Assignment-3/actions/runs/42848528926\n\n']
            if number==2:
                parts += [block(folder+'/shallow_copy.cpp'),excerpt(folder+'/outputs/sanitizer_shallow_copy_stderr.txt'),excerpt(folder+'/outputs/sanitizer_without_copy_constructor_stderr.txt')]
            if number==3:
                parts += [body('Reference/sanitizer_experiment_notes.md'),block(folder+'/destructor_experiments/missing_delete.cpp'),excerpt(folder+'/outputs/sanitizer_destructor_experiments_missing_delete_stderr.txt'),block(folder+'/destructor_experiments/mismatched_delete.cpp'),excerpt(folder+'/outputs/sanitizer_destructor_experiments_mismatched_delete_stderr.txt')]
        if number in [2,4,5,6]:
            parts += ['## Actual measured benchmark trials and evaluation\n\n',body(folder+'/benchmark_results.md')]
        if number==6:
            parts += ['## Actual copy-count/elision experiments\n\n',block(folder+'/copy_experiment.cpp'),block(folder+'/outputs/copy_experiment_actual.txt','text'),'Same source built with -fno-elide-constructors:\n\n',block(folder+'/outputs/copy_experiment_no_elide_actual.txt','text')]
        if number==7:
            parts += ['## Independent hand-derived reference and standard-library verification\n\n',block(folder+'/hand_calculation.cpp'),block(folder+'/outputs/hand_calculation_actual.txt','text'),block(folder+'/verification.cpp'),block(folder+'/outputs/verification_actual.txt','text')]
        parts += ['## Executed test cases\n\n',body(folder+'/test_cases.md'),'## Actual console output\n\n',block(folder+'/outputs/main_actual.txt','text'),'## Genuine Programiz figures\n\nEach figure is an actual Run result with source beside output. Additional middle/end views for the long primary source files are retained in screenshots/. These images supplement the readable code and explanations above. Full unchanged captures remain available in the repository.\n\n']
        for record in sorted((ROOT/folder/'outputs').glob('*_programiz.json')):
            info=json.loads(record.read_text());stem=Path(info['source']).stem;figure+=1
            parts += [f'![Figure {figure}. Q{number} - {stem}.cpp: genuine Programiz source and observed output](../{folder}/screenshots/{stem}_programiz.jpg)\n\n']
    parts += ['# Final audit and personal submission\n\n',body('SUBMISSION_PREPARATION.md'),body('ASSIGNMENT_CHECKLIST.md'),
              '## Reproduction and evidence index\n\nREADME.md gives the exact commands and purpose of every independent program. tools/run_all.py reproduces safe local programs and compile-only negatives; tools/sanitizers.py runs the isolated Linux cases; tools/summarize.py derives tables from real CSV; tools/audit.py checks source/dependency hashes,original TODO preservation,immutable predictions/chronology,actual online output and sanitizer expectations. Reference/final_audit.json contains the detailed evidence audit. VIVA_PREPARATION.md provides concept explanations,faculty questions and two practice modifications per task.\n\n',
              'The complete benchmark sources are Task02/benchmark.cpp,Task04/benchmark.cpp,Task05/benchmark.cpp,Task05/access_benchmark.cpp and Task06/benchmark.cpp. Raw trial files and method limitations are included with each task. The first no-report leak attempt remains archived and is not overwritten as a historical success.\n\n',
              'The remaining student/faculty steps are explicitly marked pending. No output,warning,sanitizer finding,measurement,screenshot,execution date or historical prediction is fabricated, and no Classroom submission is claimed.\n']
    markdown=''.join(parts)
    (REPORT/'Assignment3_Report.md').write_text(markdown,encoding='utf-8',newline='\n')
    return markdown

if __name__=='__main__':
    render_pdf(make_markdown())
    from pypdf import PdfReader
    reader=PdfReader(REPORT/'Assignment3_Report.pdf')
    print('Generated report:',len(reader.pages),'pages,',sum(len(page.images) for page in reader.pages),'genuine screenshot figures')
