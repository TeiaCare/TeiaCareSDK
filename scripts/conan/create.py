#!/usr/bin/python
import subprocess
import argparse
import pathlib
import sys
import os

def setup_conan_home():
    current_working_directory = pathlib.Path().resolve()
    os.environ['CONAN_USER_HOME'] = str(current_working_directory.absolute())
    print("CONAN_USER_HOME:", os.getenv('CONAN_USER_HOME'))

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release', 'RelWithDebInfo'])
    parser.add_argument("compiler", help="Compiler name", choices=['gcc', 'clang', 'visual_studio'])
    parser.add_argument("compiler_version", help="Compiler version")
    # parser.add_argument("-i", "--install_dir", help="Package install directory")
    return parser.parse_args()

def run(command):
    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

def conan_create(conanfile_directory, profile_path, build_type):
    command = [
        'conan', 'create', f'{conanfile_directory}', '_/_',
        '--settings', f'build_type={build_type}',
        '--profile:build', f'{profile_path}',
        '--profile:host', f'{profile_path}',
        '--build', 'missing'
    ]
    run(command)

def get_profile_path(profile_name):
    profile_path = pathlib.Path(os.getenv('CONAN_USER_HOME'), ".conan", "profiles", profile_name)
    return profile_path

def main():
    setup_conan_home()
    args = parse()

    profile_name = f'{args.compiler+args.compiler_version}'
    profile_path = get_profile_path(profile_name)
    current_working_directory = pathlib.Path().resolve()

    conan_create(current_working_directory, profile_path, args.build_type)

if __name__ == '__main__':
    sys.exit(main())
