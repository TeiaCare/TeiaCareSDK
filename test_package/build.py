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

import subprocess
import argparse
import pathlib
import sys
import os
import re

def get_project_version():
    with open('VERSION', encoding='utf8') as version_file:
        version_regex = r'^\d+\.\d+\.\d+$'
        version = version_file.read().strip()
        if re.match(version_regex, version):
            return version
        else:
            raise ValueError(f"Invalid version detected into file VERSION: {version}")

def run(command):
    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release', 'RelWithDebInfo'])
    parser.add_argument("compiler", help="Compiler name", choices=['gcc', 'clang', 'visual_studio'])
    parser.add_argument("compiler_version", help="Compiler version")
    return parser.parse_args()

def set_environment(profile_name):
    CC = subprocess.run(f'conan profile get env.CC {profile_name}', shell=True, capture_output=True).stdout.decode().strip()
    CXX = subprocess.run(f'conan profile get env.CXX {profile_name}', shell=True, capture_output=True).stdout.decode().strip()

    if not CC or not CXX:
        raise SystemError("\n========================================================"
                          "\nUnable to find CC or CXX environment variable"
                          f"\nPlease check the conan profile {profile_name}"
                          f"\nat {os.getenv('CONAN_USER_HOME')}/.conan/profiles"
                          "\n========================================================\n")

    os.environ['CC'] = CC
    os.environ['CXX'] = CXX

    print("\n========================================================")
    print("CONAN_USER_HOME:", os.getenv('CONAN_USER_HOME'))
    print("CXX:", CXX)
    print("CC:", CC)
    print("========================================================\n")


def get_profile_path(profile_name):
    profile_path = pathlib.Path(os.getenv('CONAN_USER_HOME'), ".conan", "profiles", profile_name)
    return profile_path

def conan_install(profile_name, build_type):
    command = [
        'conan', 'install', f'teiacare_sdk/{get_project_version()}@_/_',
        '--install-folder', 'build/modules',
        '--settings', f'build_type={build_type}',
        '--profile:build', f'{profile_name}',
        '--profile:host', f'{profile_name}'
    ]
    run(command)

def cmake_configure(build_type):
    command = [
        'cmake',
        '-G', 'Ninja',
        '-D', f'CMAKE_BUILD_TYPE={build_type}',
        '-D', 'TC_ENABLE_UNIT_TESTS=False',
        '-D', 'TC_ENABLE_UNIT_TESTS_COVERAGE=False',
        '-D', 'TC_ENABLE_BENCHMARKS=False',
        '-D', 'TC_ENABLE_EXAMPLES=False',
        '-D', 'TC_ENABLE_WARNINGS_ERROR=False',
        '-D', 'TC_ENABLE_SANITIZER_ADDRESS=False',
        '-D', 'TC_ENABLE_SANITIZER_THREAD=False',
        '-D', 'TC_ENABLE_CLANG_FORMAT=False',
        '-D', 'TC_ENABLE_CLANG_TIDY=False',
        '-D', 'TC_ENABLE_CPPCHECK=False',
        '-D', 'TC_ENABLE_CPPLINT=False',
        '-D', 'TC_ENABLE_DOCS=False',
        '-B', f'build/{build_type}',
        '-S', '.',
        '--fresh'
    ]
    run(command)

def cmake_build(build_type):
    command = [
        'cmake',
        '--build', f'./build/{build_type}',
        '--config', f'{build_type}'
    ]
    run(command)

def cmake_install(build_type):
    command = [
        'cmake',
        '--install', f'./build/{build_type}',
        '--prefix', f'./install'
    ]
    run(command)

def main():
    args = parse()

    profile_name = f'{args.compiler+args.compiler_version}'
    profile_path = get_profile_path(profile_name)
    set_environment(profile_name)
    conan_install(profile_path, args.build_type)

    cmake_configure(args.build_type)
    cmake_build(args.build_type)
    cmake_install(args.build_type)

if __name__ == '__main__':
    sys.exit(main())
