#!/usr/bin/python
import platform
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
    parser.add_argument("-d", "--directories", help="Specific conanfiles directories", nargs='*', required=False)
    return parser.parse_args()

def run(command):
    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except Exception as e:
        print(f'Unhandled Exception: {e}')

def conan_profile_create(profile_name, args):
    run([ 'conan', 'config', 'set', 'general.revisions_enabled=1'])
    run([ 'conan', 'profile', 'new', f'{profile_name}', '--force', '--detect'])
    run([ 'conan', 'profile', 'update', f'settings.compiler={args.compiler}', f'{profile_name}' ])
    run([ 'conan', 'profile', 'update', f'settings.compiler.version={args.compiler_version}', f'{profile_name}'])
    run([ 'conan', 'profile', 'update', f'env.CONAN_CMAKE_GENERATOR=Ninja', f'{profile_name}']) # for conan v1 recipes
    run([ 'conan', 'profile', 'update', f'conf.tools.cmake.cmaketoolchain:generator=Ninja', f'{profile_name}']) # for conan v2 recipes
    run([ 'conan', 'profile', 'update', f'conf.tools.system.package_manager:mode=install', f'{profile_name}'])
    run([ 'conan', 'profile', 'update', f'conf.tools.system.package_manager:sudo=True', f'{profile_name}'])

    if args.compiler == 'clang':
        if platform.system() == 'Linux':
            cc_compiler = f'clang-{args.compiler_version}'
            cxx_compiler = f'clang++-{args.compiler_version}'
            run([ 'conan', 'profile', 'update', f'settings.compiler.libcxx=libstdc++11', f'{profile_name}'])
            run([ 'conan', 'profile', 'update', f'env.CC={cc_compiler}', f'{profile_name}'])
            run([ 'conan', 'profile', 'update', f'env.CXX={cxx_compiler}', f'{profile_name}'])

        if platform.system() == 'Darwin':
            cc_compiler = 'clang'
            cxx_compiler = 'clang++'
            run([ 'conan', 'profile', 'update', f'settings.compiler=apple-clang', f'{profile_name}' ])
            run([ 'conan', 'profile', 'update', f'settings.compiler.libcxx=libc++', f'{profile_name}'])
            run([ 'conan', 'profile', 'update', f'env.CC={cc_compiler}', f'{profile_name}'])
            run([ 'conan', 'profile', 'update', f'env.CXX={cxx_compiler}', f'{profile_name}'])

        if platform.system() == 'Windows':
            cc_compiler = 'clang-cl'
            cxx_compiler = 'clang-cl'
            run([ 'conan', 'profile', 'update', f'settings.compiler=clang', f'{profile_name}' ])
            run([ 'conan', 'profile', 'update', f'env.CC={cc_compiler}', f'{profile_name}'])
            run([ 'conan', 'profile', 'update', f'env.CXX={cxx_compiler}', f'{profile_name}'])

    if args.compiler == 'gcc':
        cc_compiler = ''
        cxx_compiler = ''

        if platform.system() == 'Linux':
            cc_compiler = f'gcc-{args.compiler_version}'
            cxx_compiler = f'g++-{args.compiler_version}'

        if platform.system() == 'Darwin':
            cc_compiler = f'gcc'
            cxx_compiler = f'g++'

        run([ 'conan', 'profile', 'update', f'settings.compiler.libcxx=libstdc++11', f'{profile_name}'])
        run([ 'conan', 'profile', 'update', f'env.CC={cc_compiler}', f'{profile_name}'])
        run([ 'conan', 'profile', 'update', f'env.CXX={cxx_compiler}', f'{profile_name}'])

    if args.compiler == 'visual_studio':
        if platform.system() != 'Windows':
            raise RuntimeError(f"Visual Studio is only supported on Windows. Current OS: {platform.system()}")

        compiler = 'cl'
        run([ 'conan', 'profile', 'update', f'settings.compiler=Visual Studio', f'{profile_name}' ])
        run([ 'conan', 'profile', 'update', f'env.CC={compiler}', f'{profile_name}'])
        run([ 'conan', 'profile', 'update', f'env.CXX={compiler}', f'{profile_name}'])

def conan_profile_show(profile_name):
    run([ 'conan', 'profile', 'show', f'{profile_name}' ])

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

def get_conanfile_directories(args):
    current_working_directory = pathlib.Path().resolve()
    conanfile_directories = []
    if not args.directories:
        conanfile_directories_include = set(current_working_directory.glob('**/conanfile.txt'))
        conanfile_directories_exclude = set(current_working_directory.glob('**/.conan/**/conanfile.*'))
        conanfile_directories = conanfile_directories_include - conanfile_directories_exclude
    else:
        conanfile_directories = args.directories

    return conanfile_directories

def main():
    setup_conan_home()
    args = parse()

    profile_name = f'{args.compiler+args.compiler_version}'
    conan_profile_create(profile_name, args)
    conan_profile_show(profile_name)

    conanfile_directories = get_conanfile_directories(args)
    for conanfile_directory in conanfile_directories:
        print("\nProcessing directory:", conanfile_directory)
        conan_install(conanfile_directory, profile_name, args.build_type)

if __name__ == '__main__':
    sys.exit(main())
