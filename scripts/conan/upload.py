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
    parser.add_argument("version_file", help="Path to the package version file")
    parser.add_argument("remote_name", help="Conan remote name")
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release'])
    parser.add_argument("compiler", help="Compiler name", choices=['gcc', 'clang', 'visual_studio'])
    parser.add_argument("compiler_version", help="Compiler version")
    return parser.parse_args()

def run(command):
        ret = subprocess.run(command)
        ret.check_returncode()

def conan_create(remote_name, package_version, build_type, compiler, compiler_version):
    if package_version.startswith('v'):
        package_version = package_version[1:]
    command = [
        'conan', 'upload', '-c', '-r', remote_name, f'teiacore_sdk/{package_version}', '-q', f'\'build_type={build_type} AND compiler={compiler}{compiler_version}\''
    ]
    run(command)

def get_profile_path(profile_name):
    profile_path = pathlib.Path(os.getenv('CONAN_USER_HOME'), ".conan", "profiles", profile_name)
    return profile_path

def main():
    setup_conan_home()
    args = parse()
    
    version_file = open(args.version_file, "r")
    package_version = version_file.read()

    conan_create(args.remote_name, package_version, args.build_type, args.compiler, args.compiler_version)

if __name__ == '__main__':
    sys.exit(main())
