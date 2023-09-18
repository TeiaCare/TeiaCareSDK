#!/usr/bin/python
import argparse
from command import run

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("program_path", help="Path to the program to sanitize")
    parser.add_argument("program_args", help="Optional arguments to the sanitized program", nargs='*')
    
    parser.add_argument("--address_sanitizer", required=False, default=False, action='store_true')
    parser.add_argument("--asan_log_path", help="Address Sanitizer log path", default='./results/sanitizers/address_sanitizer/asan.log')
    
    parser.add_argument("--thread_sanitizer", required=False, default=False, action='store_true')
    parser.add_argument("--tsan_log_path", help="Thread Sanitizer log path", default='./results/sanitizers/thread_sanitizer/tsan.log')
    
    args, program_args = parser.parse_known_args()
    return args, program_args

def sanitizer(args, program_args):
    if args.address_sanitizer:
        address_sanitizer(args, program_args)

    if args.thread_sanitizer:
        thread_sanitizer(args, program_args)

def address_sanitizer(args, program_args):
    cmd = [ args.program_path ]
    cmd.extend(program_args)
    run(cmd, env_vars={'TSAN_OPTIONS': f'exitcode=2 verbosity=1 log_path={args.tsan_log_path}'}, debug=True)

def thread_sanitizer(args, program_args):
    cmd = [ args.program_path ]
    cmd.extend(program_args)
    run(cmd, env_vars={'ASAN_OPTIONS': f'exitcode=2 verbosity=1 log_path={args.asan_log_path}'}, debug=True)

if __name__ == '__main__':
    args, program_args = parse()
    sanitizer(args, program_args)