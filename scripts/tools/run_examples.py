#!/usr/bin/python
import argparse
from command import run
import pathlib

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)    
    parser.add_argument("examples_directory", help="Examples Directory")
    return parser.parse_args()

def examples(args):
    examples_path = pathlib.Path(args.examples_directory).absolute()
    if not examples_path.exists() or not examples_path.is_dir():
        print("Examples directory must be an existing directory")
        return

    for example in examples_path.iterdir():
        print('#'*80 + f'\n{example.name}\n' + '#'*80)
        run(example)

if __name__ == '__main__':
    examples(parse())
