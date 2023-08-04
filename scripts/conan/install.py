#!/usr/bin/python
import subprocess
import argparse
import pathlib
import os

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release'])
    parser.add_argument("profile", help="Conan Profile")
    parser.add_argument("-d", "--directories", help="Specific conanfiles directories", nargs='*', required=False)
    return parser.parse_args()

def get_profile_path(args):
    conan_directory = pathlib.Path(__file__).resolve().parent.parent
    profile_path = pathlib.PurePath(conan_directory, "profiles", f'{args.profile}')
    return profile_path

def conan_install(conanfile_directory, profile_path, args):
    command = [
        'conan', 'install', f'{conanfile_directory}',
        '--install-folder', f'build/modules', 
        '--settings', f'build_type={args.build_type}',
        '--profile:build', f'{profile_path}',
        '--profile:host', f'{profile_path}',
        '--build', 'missing'
    ]

    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

args = parse()
profile_path = get_profile_path(args)

path = pathlib.Path().resolve()
os.environ['CONAN_USER_HOME'] = str(path.absolute())

conanfile_directories = []
if not args.directories:
    conanfile_directories_include = set(path.glob('*/conanfile.txt'))
    conanfile_directories_exclude = set(path.glob('**/.conan/**/conanfile.*'))
    conanfile_directories = conanfile_directories_include - conanfile_directories_exclude
else:
    conanfile_directories = args.directories

for conanfile_directory in conanfile_directories:
    print("Processing directory:", conanfile_directory)
    conan_install(conanfile_directory, profile_path, args)
