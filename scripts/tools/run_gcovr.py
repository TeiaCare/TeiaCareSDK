#!/usr/bin/python
import subprocess
import argparse
import pathlib

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("compiler", help="Compiler name", choices=['gcc', 'clang', 'apple-clang', 'Visual Studio'])
    parser.add_argument("compiler_version", help="Compiler version")
    return parser.parse_args()

def run(args):
    coverage_tool = None

    if args.compiler == 'gcc':
        coverage_tool = f'gcov-{args.compiler_version}'
    
    if args.compiler == 'clang' or args.compiler == 'apple-clang':
        coverage_tool = f'llvm-cov-{args.compiler_version} gcov'

    if args.compiler == 'Visual Studio':
        raise ValueError(f'Unsupported compiler for code coverage: {args.compiler}')

    cwd = pathlib.Path().absolute().resolve()
    coverage_directory = pathlib.Path(cwd, "unit_tests/coverage/")
    coverage_directory.mkdir(parents=True, exist_ok=True)

    command = [
        'gcovr',
        '-r',
        '.',
        '--xml',
        '--xml-pretty',
        '--output', f'{coverage_directory}/cobertura.xml',
        '--html-details', f'{coverage_directory}/results.html',
        '--html-title', 'event_dispatcher',
        '--filter', 'event_dispatcher/.*',
        '--gcov-executable', f'{coverage_tool}',
        '--exclude-unreachable-branches',
        '--exclude-throw-branches'
    ]

    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

args = parse()
run(args)