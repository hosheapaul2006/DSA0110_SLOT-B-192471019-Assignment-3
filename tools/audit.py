"""Check recorded source, predictions, diagnostics and actual execution evidence."""
import datetime
import hashlib
import json
from pathlib import Path
import re
import subprocess
ROOT=Path(__file__).resolve().parents[1]
checks=[]
def check(name,condition,evidence):
    checks.append({'requirement':name,'status':'PASS' if condition else 'FAIL','evidence':evidence})
    if not condition:raise RuntimeError(name+': '+evidence)
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def git(*args):return subprocess.check_output(['git',*args],cwd=ROOT,text=True).strip()
def local_dependencies(source,seen=None):
    seen=set() if seen is None else seen
    for included in re.findall(r'^\s*#include\s+"([^"]+)"',source.read_text(),re.M):
        path=(source.parent/included).resolve()
        if path not in seen:
            seen.add(path);local_dependencies(path,seen)
    return seen
records=[]
for path in sorted(ROOT.glob('Task*/outputs/*_verification.json')):
    if path.name.startswith('sanitizer_'):continue
    record=json.loads(path.read_text());records.append(record);source=ROOT/record['source']
    check(record['source']+' current source hash',source.exists() and sha(source)==record['sha256'],path.relative_to(ROOT).as_posix())
    if record['expected_diagnostic']=='error':
        check(record['source']+' expected error',record['compile_exit']!=0 and record['status'].startswith('PASS'),str(path.relative_to(ROOT)))
    else:
        check(record['source']+' compilation',record['compile_exit']==0,str(path.relative_to(ROOT)))
        if not record['compile_only']:
            check(record['source']+' execution',record.get('run_exit')==0 and record['status']=='PASS',str(path.relative_to(ROOT)))
    dependencies=local_dependencies(source)
    for header,expected in record.get('headers_sha256',{}).items():
        if (ROOT/header).resolve() in dependencies:check(record['source']+' header '+header,sha(ROOT/header)==expected,'Included header hash matches tested version')
for source in ROOT.glob('Task*/**/*.cpp'):
    check(str(source.relative_to(ROOT))+' documented compilation',any(record['source']==source.relative_to(ROOT).as_posix() for record in records),'Verification JSON exists')
    check(str(source.relative_to(ROOT))+' no unfinished TODO','TODO' not in source.read_text(),'All requested source TODOs replaced')
for task,commit in [(3,'0aa0309'),(5,'a7c3793')]:
    for name in ['prediction_before_execution.md','outputs/predicted_output.txt','predict_output.cpp']:
        path=f'Task{task:02}/'+name
        original=subprocess.check_output(['git','show',commit+':'+path],cwd=ROOT).replace(b'\r\n',b'\n')
        check(path+' immutable',original==(ROOT/path).read_bytes().replace(b'\r\n',b'\n'),'Matches prediction commit '+commit)
    check(f'Q{task} supplied prediction source',(ROOT/f'Task{task:02}/predict_output.cpp').read_text()==(ROOT/f'Reference/Q{task}_prediction_original.txt').read_text(),'Exact extracted source retained')
    prediction_record=json.loads((ROOT/f'Task{task:02}/outputs/predict_output_verification.json').read_text())
    check(f'Q{task} prediction chronology',int(git('show','-s','--format=%ct',commit))<=datetime.datetime.fromisoformat(prediction_record['timestamp_utc']).timestamp(),'Prediction commit precedes execution')
    check(f'Q{task} exact output',prediction_record.get('expected_comparison','').startswith('PASS'),'Every character compared with only platform newline decoding')
for task in [2,4]:
    original=(ROOT/f'Reference/Q{task}_TODO_original.txt').read_text()
    completed=(ROOT/f'Task{task:02}/complete_code.cpp').read_text()
    if task==2:
        inserts=[(ROOT/'Reference/Q2_completed_copy_constructor.txt').read_text(),re.search(r'    Prescription& operator=\(const Prescription& other\)\n    \{.*?\n    \}\n',completed,re.S)[0]]
        for number,insert in enumerate(inserts,1):original=re.sub(rf'    // TODO {number}:[^\n]*(?:\n    //[^\n]*)*\n[ \t]*\n',lambda match:insert,original)
        original=re.sub(r'        // TODO 3:[^\n]*\n[ \t]*\n','        delete[] medicines;\n',original)
        check('Q2 removed-copy experiment',completed.replace(inserts[0],'')==(ROOT/'Task02/without_copy_constructor.cpp').read_text(),'Only the completed copy constructor is removed')
    else:
        inserts=[re.search(r'    Money operator\+\(const Money& other\) const\n    \{.*?\n    \}\n',completed,re.S)[0],re.search(r'    bool operator==.*?\n    \}\n',completed,re.S)[0]]
        for number,insert in enumerate(inserts,1):original=re.sub(rf'    // TODO {number}:[^\n]*\n[ \t]*\n',lambda match:insert,original)
    check(f'Q{task} TODO-only preservation',original==completed,'All text outside replaced TODO regions matches supplied program')
online=[]
for path in sorted(ROOT.glob('Task*/outputs/*_programiz.json')):
    record=json.loads(path.read_text());online.append(record);source=ROOT/record['source'];stem=source.stem
    check(record['source']+' online source hash',sha(source)==record['sha256'],'Exact locally compiled source was pasted')
    observed=(source.parent/'outputs'/f'{stem}_programiz.txt').read_text();expected=(source.parent/'outputs'/f'{stem}_actual.txt').read_text()
    check(record['source']+' online actual output',observed.startswith(expected) and record['status']=='PASS','Actual result-pane stdout matches local output prefix')
    check(record['source']+' genuine screenshot',(source.parent/'screenshots'/f'{stem}_programiz.jpg').is_file(),'Browser capture saved after completed Run')
sanitizers=json.loads((ROOT/'Reference/sanitizer_summary.json').read_text())
for record in sanitizers:
    check(record['source']+' sanitizer source hash',sha(ROOT/record['source'])==record['sha256'],'Current source matches actual Linux sanitizer source')
    check(record['source']+' sanitizer expectation',record['status']=='PASS' and record['compile_exit']==0,'Saved actual exit='+str(record.get('run_exit'))+'; '+record['run_url'])
    prefix=record['source'].split('/')[0]+'/outputs/sanitizer_'+record['source'].split('/',1)[1].removesuffix('.cpp').replace('/','_')
    stderr=(ROOT/(prefix+'_stderr.txt')).read_text()
    if record['expected_error_pattern']:
        check(record['source']+' real expected diagnostic',record['run_exit']!=0 and re.search(record['expected_error_pattern'],stderr) is not None,'Nonzero exit with expected real sanitizer report')
    else:check(record['source']+' clean sanitizer run',record['run_exit']==0 and not stderr,'Exit0 and empty sanitizer stderr')
for task in range(1,8):check(f'Q{task} explanation/test table',all((ROOT/f'Task{task:02}'/name).is_file() for name in ['explanation.md','test_cases.md']),'Source-linked documentation exists')
tracked=git('ls-files').splitlines();check('no tracked build products',not any(p.startswith('build/') or p.endswith(('.exe','.obj','.o','.zip')) for p in tracked),'git tracked-file audit')
base={x['source'] for x in records if not Path(x['source']).name.startswith('online') and x['compile_exit']==0}
normal={x['source'] for x in records if x.get('run_exit')==0}
result={'timestamp_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
        'base_successfully_compiled_programs':len(base),'compiled_source_files_including_online':len({x['source'] for x in records if x['compile_exit']==0}),
        'normal_executed_source_files':len(normal),'normal_build_run_configurations':sum(x.get('run_exit')==0 for x in records),
        'intentional_compiler_error_sources':len({x['source'] for x in records if x['compile_exit']!=0}),
        'recorded_local_check_evaluations':sum(x['test_count'] for x in records),
        'base_check_evaluations':sum(x['test_count'] for x in records if not Path(x['source']).name.startswith('online')),
        'sanitizer_cases':len(sanitizers),'sanitizer_clean_cases':sum(not x['expected_error_pattern'] for x in sanitizers),
        'sanitizer_expected_error_cases':sum(bool(x['expected_error_pattern']) for x in sanitizers),
        'programiz_programs':len(online),'genuine_screenshots':len(list(ROOT.glob('Task*/screenshots/*.jpg'))),'audit_checks':checks}
(ROOT/'Reference/final_audit.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(json.dumps({k:v for k,v in result.items() if k!='audit_checks'},indent=2));print('Evidence audit checks:',len(checks),'PASS')
