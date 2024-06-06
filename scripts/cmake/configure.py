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
import subprocess
import os
import sys
from .command import run, check_venv

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type",           choices=['Debug', 'Release', 'RelWithDebInfo'])
    parser.add_argument("compiler",             help="Compiler name", choices=['gcc', 'clang', 'visual_studio'])
    parser.add_argument("compiler_version",     help="Compiler version")
    parser.add_argument("--build_dir",          required=False, default='./build')
    parser.add_argument("--integration_tests",  required=False, default=False, action='store_true')
    parser.add_argument("--unit_tests",         required=False, default=False, action='store_true')
    parser.add_argument("--coverage",           required=False, default=False, action='store_true')
    parser.add_argument("--benchmarks",         required=False, default=False, action='store_true')
    parser.add_argument("--examples",           required=False, default=False, action='store_true')
    parser.add_argument("--warnings",           required=False, default=False, action='store_true')
    parser.add_argument("--address_sanitizer",  required=False, default=False, action='store_true')
    parser.add_argument("--thread_sanitizer",   required=False, default=False, action='store_true')
    parser.add_argument("--clang_format",       required=False, default=False, action='store_true')
    parser.add_argument("--clang_tidy",         required=False, default=False, action='store_true')
    parser.add_argument("--cppcheck",           required=False, default=False, action='store_true')
    parser.add_argument("--cpplint",            required=False, default=False, action='store_true')
    parser.add_argument("--docs",               required=False, default=False, action='store_true')
    args, _ = parser.parse_known_args()
    return args

def main():
    args = parse()
    profile_name = f'{args.compiler+args.compiler_version}'

    CC = subprocess.run(f'conan profile get env.CC {profile_name}', shell=True, capture_output=True).stdout.decode().strip()
    CXX = subprocess.run(f'conan profile get env.CXX {profile_name}', shell=True, capture_output=True).stdout.decode().strip()

    if not CC or not CXX:
        raise SystemError("\n========================================================"
                          "\nUnable to find CC or CXX environment variable"
                          f"\nPlease check the conan profile {profile_name}"
                          f"\nat {os.getenv('CONAN_USER_HOME')}/.conan/profiles"
                          "\n========================================================\n")

    os.environ['CC'] = str(CC)
    os.environ['CXX'] = str(CXX)

    print("\n========================================================")
    print("CONAN_USER_HOME:", os.getenv('CONAN_USER_HOME'))
    print("CXX:", os.getenv('CXX'))
    print("CC:", os.getenv('CC'))
    print("========================================================\n")

    run([
        'cmake',
        '-G', 'Ninja',
        '-D', f'CMAKE_BUILD_TYPE={str(args.build_type)}',
        '-D', f'TC_ENABLE_UNIT_TESTS={str(args.unit_tests or args.coverage)}',
        '-D', f'TC_ENABLE_UNIT_TESTS_COVERAGE={str(args.coverage)}',
        '-D', f'TC_ENABLE_BENCHMARKS={str(args.benchmarks)}',
        '-D', f'TC_ENABLE_EXAMPLES={str(args.examples)}',
        '-D', f'TC_ENABLE_WARNINGS_ERROR={str(args.warnings)}',
        '-D', f'TC_ENABLE_SANITIZER_ADDRESS={str(args.address_sanitizer)}',
        '-D', f'TC_ENABLE_SANITIZER_THREAD={str(args.thread_sanitizer)}',
        '-D', f'TC_ENABLE_CLANG_FORMAT={str(args.clang_format)}',
        '-D', f'TC_ENABLE_CLANG_TIDY={str(args.clang_tidy)}',
        '-D', f'TC_ENABLE_CPPCHECK={str(args.cppcheck)}',
        '-D', f'TC_ENABLE_CPPLINT={str(args.cpplint)}',
        '-D', f'TC_ENABLE_DOCS={str(args.docs)}',
        '-B', f'{args.build_dir}/{args.build_type}',
        '-S', '.',
        '--fresh'
    ])

if __name__ == '__main__':
    check_venv()
    sys.exit(main())
