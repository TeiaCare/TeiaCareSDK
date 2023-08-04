#!/usr/bin/python
import subprocess
import argparse
import pathlib

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release'])
    return parser.parse_args()

def run(args):
    cwd = pathlib.Path().absolute().resolve()
    results_directory = pathlib.Path(cwd, "unit_tests/results/")
    results_directory.mkdir(parents=True, exist_ok=True)

    command = [
        'ctest',
        '--parallel8',
        '--test-dir', f'./build/{args.build_type}',
        '--output-junit', f'{results_directory}/tests.xml',
        '--output-on-failure',
    ]

    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

args = parse()
run(args)