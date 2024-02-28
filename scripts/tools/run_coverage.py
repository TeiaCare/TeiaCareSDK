#!/usr/bin/python
import argparse
from command import run, create_directory

def check():
    run(['gcovr', '--version'])

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("compiler", help="Compiler name", choices=['gcc', 'clang', 'visual_studio'])
    parser.add_argument("compiler_version", help="Compiler version")
    parser.add_argument("--xml_coverage_path", help="Coverage xml results path", required=False, default='./results/coverage/cobertura.xml')
    parser.add_argument("--html_coverage_path", help="Coverage html results path", required=False, default='./results/coverage/html/coverage.html')
    return parser.parse_args()

def coverage(args):    
    coverage_tool = None
    if args.compiler == 'gcc':
        coverage_tool = f'gcov-{args.compiler_version}'    
    if args.compiler == 'clang' or args.compiler == 'apple-clang':
        coverage_tool = f'llvm-cov-{args.compiler_version} gcov'
    if args.compiler == 'msvc':
        raise ValueError(f'Unsupported compiler for code coverage: {args.compiler}.')

    create_directory(args.xml_coverage_path)
    create_directory(args.html_coverage_path)

    run([
        'gcovr', '-r', '.',
        '--xml', '--xml-pretty', '--output', args.xml_coverage_path,
        '--html-title', 'TeiaCareSDK', '--html-details', args.html_coverage_path,
        '--filter', 'sdk/.*',
        '--exclude', 'sdk/include/teiacare/sdk/datetime/date/.*',
        '--exclude', 'sdk/tests/.*',
        '--gcov-executable', coverage_tool,
        '--exclude-unreachable-branches',
        '--exclude-throw-branches'
    ])

if __name__ == '__main__':
    check()
    coverage(parse())
