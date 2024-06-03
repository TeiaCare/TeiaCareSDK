#!/usr/bin/python
import argparse
from command import run
import pathlib
import os

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("examples_directory", help="Examples Directory")
    return parser.parse_args()

def is_executable(file_path):
    return os.access(file_path, os.X_OK)

def print_example_name(example_name):
    line_long = '#'*80
    line_short = '#'*3
    print(f'\n\n{line_long}\n{line_short}', flush=True)
    print(f'{line_short} {example_name}', flush=True)
    print(f'{line_short}\n{line_long}', flush=True)

def examples(args):
    examples_path = pathlib.Path(args.examples_directory).absolute()
    if not examples_path.exists() or not examples_path.is_dir():
        print("Examples directory must be an existing directory")
        return

    examples_found = False
    for example in examples_path.iterdir():
        if example.is_file() and is_executable(example):
            examples_found = True
            print_example_name(example.name)
            run(example)

    if not examples_found:
        raise RuntimeError(f'No examples found into {examples_path}')

if __name__ == '__main__':
    examples(parse())
