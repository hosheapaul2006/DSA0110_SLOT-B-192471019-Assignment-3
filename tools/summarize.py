"""Summarize actual captured benchmark CSV; never generate timing observations."""
import csv
import json
from pathlib import Path
import statistics
ROOT=Path(__file__).resolve().parents[1]
summary={}
for task in [2,4,5,6]:
    folder=ROOT/f'Task{task:02}'
    lines=['# Actual benchmark results','', 'Local Windows GCC14.2.0. Each configuration has five actual trials. Milliseconds include the operations described in explanation.md. Raw CSV and compiler/source/exit records are retained in outputs/.','']
    for path in sorted((folder/'outputs').glob('*benchmark*_actual.txt')):
        rows=list(csv.DictReader(path.read_text().splitlines()));groups={}
        lines+=['## '+path.stem.removesuffix('_actual'),'', '| Workload | Actual trials (ms) | Median (ms) |','|---|---|---|']
        for row in rows:
            key=row.get('design') or ('medicines='+row['medicines']+', copies='+row['copies'])
            groups.setdefault(key,[]).append(float(row['milliseconds']))
        for group,values in groups.items():
            median=statistics.median(values)
            lines.append('| '+group+' | '+', '.join(f'{v:.6f}' for v in values)+f' | {median:.6f} |')
            summary[f'Task{task:02}/'+path.stem.removesuffix('_actual')+'/'+group]={'trials_ms':values,'median_ms':median}
        if task==2:
            lines+=['', 'Median complete copy lifetime per copy (allocation, content traversal and destruction included):']
            for group,values in groups.items():
                copies=int(group.split('copies=')[1]);microseconds=statistics.median(values)*1000/copies
                lines.append(f'- {group}: {microseconds:.6f} microseconds/copy.')
                summary[f'Task{task:02}/'+path.stem.removesuffix('_actual')+'/'+group]['microseconds_per_copy']=microseconds
        if task==4:
            first={row['design']:row for row in rows if row['trial']=='1'}
            lines+=['',f"Numerical result: exact integer rupees={first['integer_paise']['sum_rupees']}; double rupees={first['double_rupees']['sum_rupees']}; observed error={first['double_rupees']['error_rupees']} rupees."]
        if task==6:
            lines+=['', 'Trial1 custom allocation counts: '+', '.join(row['design']+'='+row['custom_allocation_count'] for row in rows if row['trial']=='1')+'. std::string allocation counts are not instrumented.']
        lines+=['']
    lines+=['These are measured implementation/workload results. Loop/index work, allocation, copy elision, cache locality, guard costs and scheduler noise can affect them. Big-O and observed elapsed times are reported separately.']
    (folder/'benchmark_results.md').write_text('\n'.join(lines)+'\n',encoding='utf-8',newline='\n')
(ROOT/'Reference/benchmark_summary.json').write_text(json.dumps(summary,indent=2)+'\n',encoding='utf-8')
print('Summarized actual benchmark configurations:',len(summary))
for key,value in summary.items():
    if '_O2/' in key:print(key,round(value['median_ms'],6),value.get('microseconds_per_copy',''))
