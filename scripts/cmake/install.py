#!/usr/bin/python
import argparse
import sys
from command import run, check_venv

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", choices=['Debug', 'Release'])
    parser.add_argument("--build_dir", help="Build Directory", required=False, default='./build')
    parser.add_argument("--install_dir", help="Install Directory", required=False, default='./install')
    return parser.parse_args()

def main():
    check_venv()
    args = parse()

    run([
        'cmake',
        '--install', f'{args.build_dir}/{args.build_type}',
        '--prefix', args.install_dir
    ])

if __name__ == '__main__':
    sys.exit(main())