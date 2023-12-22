#!/usr/bin/python
import argparse
import sys
from command import run, check_venv

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", choices=['Debug', 'Release'])
    parser.add_argument("--build_dir", help="Build Directory", required=False, default='./build')
    return parser.parse_args()

def main():
    check_venv()
    args = parse()

    run([
        'cmake',
        '--build', f'{args.build_dir}/{args.build_type}',
        '--parallel', '16'
    ])

if __name__ == '__main__':
    sys.exit(main())