#!/usr/bin/python
import argparse
from command import run

def check():
    run(['cmake', '--version'])

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--build_dir", help="Build Directory", required=False, default='./build')
    parser.add_argument("--install_dir", help="Install Directory", required=False, default='./install')
    return parser.parse_args()

def install(args):
    run([
        'cmake',
        '--install', args.build_dir,
        '--prefix', args.install_dir
    ])

if __name__ == '__main__':
    check()
    install(parse())