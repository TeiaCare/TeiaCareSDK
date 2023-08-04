#!/usr/bin/python
import subprocess
import argparse
import pathlib
import sys


def run(command):
    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release'])
    parser.add_argument("profile", help="Conan Profile")
    return parser.parse_args()

def get_profile_path(args):
    conan_directory = pathlib.Path(__file__).resolve().parent
    profile_path = pathlib.PurePath(conan_directory, "profiles", f'{args.profile}')
    return profile_path

def conan_install(profile_path, args):
    conanfile_directory = pathlib.Path(__file__).resolve().parent
    command = [
        'conan', 'install', f'{conanfile_directory}',
        '--install-folder', f'build/modules', 
        '--settings', f'build_type={args.build_type}',
        '--profile', f'{profile_path}',
        '--build', 'missing'
    ]
    run(command)

def cmake_configure(args):
    command = [
        'cmake',
        '-G', 'Ninja',
        '-D', f'CMAKE_BUILD_TYPE={args.build_type}', 
        '-B', f'build/{args.build_type}',
        '-S', '.'
    ]
    run(command)

def cmake_build(args):
    command = [
        'cmake',
        '--build', f'./build/{args.build_type}',
        '--config', f'{args.build_type}'
    ]
    run(command)

def cmake_install(args):
    command = [
        'cmake',
        '--install', f'./build/{args.build_type}',
        '--prefix', f'./install/{args.build_type}'
    ]
    run(command)

def main(args):
    profile_path = get_profile_path(args)
    conan_install(profile_path, args)
    cmake_configure(args)
    cmake_build(args)
    cmake_install(args)

if __name__ == '__main__':
    args = parse()
    sys.exit(main(args))

# Run from sdk_integration_test directory:
# python build.py <Debug|Release> <PROFILE_NAME>