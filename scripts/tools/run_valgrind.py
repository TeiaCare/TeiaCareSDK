#!/usr/bin/python
# Copyright 2024 TeiaCare
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
from command import run, create_directory

def check():
    run(['valgrind', '--version'])

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("program_path", help="Path to the program to analyze")
    parser.add_argument("program_args", help="Optional arguments to the analyzed program", nargs='*')

    parser.add_argument("--memcheck", required=False, default=False, action='store_true')
    parser.add_argument("--memcheck_log_path", help="Valgrind memcheck log path", required=False, default='./results/memcheck/memcheck.log')
    parser.add_argument("--memcheck_output_path", help="Valgrind memcheck output path", required=False, default='./results/memcheck/memcheck.xml')
    parser.add_argument("--memcheck_suppression_path", help="Valgrind memcheck suppression path", required=False, default='./scripts/valgrind/suppressions')

    parser.add_argument("--callgrind", required=False, default=False, action='store_true')
    parser.add_argument("--callgrind_log_path", help="Valgrind callgrind log path", required=False, default='./results/callgrind/callgrind.log')
    parser.add_argument("--callgrind_output_path", help="Valgrind callgrind output path", required=False, default='./results/callgrind/callgrind.out')

    args, program_args = parser.parse_known_args()
    return args, program_args

def valgrind(args, program_args):
    if args.memcheck:
        memcheck(args, program_args)
    elif args.callgrind:
        callgrind(args, program_args)
    else:
        raise ValueError("Select either --memcheck or --callgrind")

def memcheck(args, program_args):
    create_directory(args.memcheck_log_path)
    create_directory(args.memcheck_output_path)

    cmd = [
        'valgrind',
        '--tool=memcheck',
        '--verbose',
        '--leak-check=full',
        '--show-leak-kinds=all',
        '--track-origins=yes',
        '--error-exitcode=1',
        '--demangle=yes',
        f'--suppressions={args.memcheck_suppression_path}',
        '--xml=yes',
        f'--xml-file={args.memcheck_output_path}',
        f'--log-file={args.memcheck_log_path}',
        '--child-silent-after-fork=yes',
        args.program_path
    ]
    # '--gen-suppressions=all',

    cmd.extend(program_args)
    run(cmd, debug=True)

def callgrind(args, program_args):
    create_directory(args.callgrind_output_path)
    create_directory(args.callgrind_log_path)

    cmd = [
        'valgrind',
        '--tool=callgrind',
        '--verbose',
        f'--callgrind-out-file={args.callgrind_output_path}',
        f'--log-file={args.callgrind_log_path}',
        '--error-exitcode=1',
        args.program_path
    ]

    cmd.extend(program_args)
    run(cmd, debug=True)

if __name__ == '__main__':
    check()
    args, program_args = parse()
    valgrind(args, program_args)
