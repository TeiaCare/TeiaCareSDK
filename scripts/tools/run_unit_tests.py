#!/usr/bin/python
import argparse
from command import run

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    # parser.add_argument("program_path", help="Path to the test program")
    parser.add_argument("--test_dir", help="Unit Tests root directory", required=False, default='./build')
    parser.add_argument("--xml_results_path", help="Unit Tests xml results path", required=False, default='./results/unit_tests/unit_tests.xml') 
    return parser.parse_args()

def run_ctests(args):
    run([
        'ctest',
        '--parallel', '4',
        '--test-dir', args.test_dir,
        '--output-junit', args.xml_results_path,
        '--timeout', '30', # seconds
        '--output-on-failure',
        '--progress',
        '--schedule_random'
    ], debug=True)

def unit_tests(args):    
    run([
        args.program_path,
        '--gtest_shuffle',
        '--gtest_random_seed=0',
        '--gtest_brief=0',
        '--gtest_print_time=1',
        f'--gtest_output=xml:{args.xml_results_path}'
    ], check_returncode=False)

if __name__ == '__main__':
    run(['ctest', '--version'])
    # run_ctests(parse())
    # unit_tests(parse())
