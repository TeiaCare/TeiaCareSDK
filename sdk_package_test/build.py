#!/usr/bin/python
import subprocess
import argparse
import pathlib
import sys
import os


def run(command):
    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release'])
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

def conan_install(conanfile_directory, profile_name, build_type):
    command = [
        'conan', 'install', f'{conanfile_directory}',
        '--install-folder', f'build/modules', 
        '--settings', f'build_type={build_type}',
        '--profile:build', f'{profile_name}',
        '--profile:host', f'{profile_name}',
        '--build', 'missing'
    ]
    run(command)

def cmake_configure(build_type):
    command = [
        'cmake',
        '-G', 'Ninja',
        '-D', f'CMAKE_BUILD_TYPE={build_type}', 
        '-B', f'build/{build_type}',
        '-S', '.'
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
    conanfile_directory = pathlib.Path(__file__).resolve().parent
    conan_install(conanfile_directory, profile_path, args.build_type)

    cmake_configure(args.build_type)
    cmake_build(args.build_type)
    cmake_install(args.build_type)

if __name__ == '__main__':
    sys.exit(main())
