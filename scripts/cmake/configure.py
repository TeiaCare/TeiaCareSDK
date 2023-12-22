#!/usr/bin/python
import argparse
import subprocess
import os
import sys
from command import run, check_venv

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type",           choices=['Debug', 'Release'])
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
    return parser.parse_args()

def main():
    check_venv()
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
    
    print("\n========================================================")
    print("CONAN_USER_HOME:", os.getenv('CONAN_USER_HOME'))
    print("CXX:", CXX)
    print("CC:", CC)
    print("========================================================\n")
    
    run([
        'cmake',
        '-G', 'Ninja',
        '-D', f'CMAKE_BUILD_TYPE={str(args.build_type)}',
        '-D', f'TEIACORE_ENABLE_INTEGRATION_TESTS={args.integration_tests}',
        '-D', f'TEIACORE_ENABLE_UNIT_TESTS={args.unit_tests}',
        '-D', f'TEIACORE_ENABLE_UNIT_TESTS_COVERAGE={str(args.coverage)}',
        '-D', f'TEIACORE_ENABLE_BENCHMARKS={str(args.benchmarks)}',
        '-D', f'TEIACORE_ENABLE_EXAMPLES={str(args.examples)}',
        '-D', f'TEIACORE_ENABLE_WARNINGS_ERROR={str(args.warnings)}',
        '-D', f'TEIACORE_ENABLE_SANITIZER_ADDRESS={str(args.address_sanitizer)}',
        '-D', f'TEIACORE_ENABLE_SANITIZER_THREAD={str(args.thread_sanitizer)}',
        '-D', f'TEIACORE_ENABLE_CLANG_FORMAT={str(args.clang_format)}',
        '-D', f'TEIACORE_ENABLE_CLANG_TIDY={str(args.clang_tidy)}',
        '-D', f'TEIACORE_ENABLE_CPPCHECK={str(args.cppcheck)}',
        '-D', f'TEIACORE_ENABLE_CPPLINT={str(args.cpplint)}',
        '-D', f'TEIACORE_ENABLE_DOCS={str(args.docs)}',
        '-D', f'TEIACORE_ENABLE_DEPENDENCIES_SETUP=False',
        '-B', args.build_dir,
        '-S', '.'
    ])

if __name__ == '__main__':
    sys.exit(main())
