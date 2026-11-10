"""Reproduce safe local executions and compiler-only negative experiments."""
from verify import ROOT,verify
for folder in sorted(ROOT.glob('Task[0-9][0-9]')):
    for path in sorted(folder.rglob('*.cpp')):
        source=path.relative_to(folder).as_posix()
        if path.name in ['missing_default.cpp','explicit_rejected.cpp']:
            verify(folder.name,source,diagnostic='error',needle='no matching' if path.name=='missing_default.cpp' else 'convert');continue
        unsafe=path.name in ['shallow_copy.cpp','without_copy_constructor.cpp','missing_delete.cpp','mismatched_delete.cpp']
        if unsafe:
            verify(folder.name,source,compile_only=True,diagnostic='warning' if path.name=='mismatched_delete.cpp' else None,needle='mismatched' if path.name=='mismatched_delete.cpp' else None);continue
        expected='outputs/predicted_output.txt' if path.name=='predict_output.cpp' else 'outputs/complete_expected.txt' if path.name=='complete_code.cpp' else None
        diagnostic='warning' if path.name=='reorder.cpp' else None
        text=path.read_text();logged='Tests tests(argc,argv)' in text
        for opt in ['-O0','-O2'] if 'benchmark' in path.name else ['-O0']:
            verify(folder.name,source,opt,expected,diagnostic,'reorder' if diagnostic else None,False,logged)
        if folder.name=='Task06' and path.name=='copy_experiment.cpp':
            verify(folder.name,source,tests=True,extra=['-fno-elide-constructors'],tag='no_elide')
