#!/usr/bin/python
import argparse
from command import run

def check():
    run(['cmake', '--version'])

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--build_dir", help="Build Directory", required=False, default='./build')
    return parser.parse_args()

def build(args):
    run([
        'cmake',
        '--build', args.build_dir,
        '--parallel', '16'
    ])

if __name__ == '__main__':
    check()
    build(parse())