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

import sys
import concurrent.futures
import subprocess
import pathlib
import argparse

APPLICATION_NAME = "clang-format runner"

def parse():
    parser = argparse.ArgumentParser(APPLICATION_NAME, formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("directory", help="Directory to look for files, of the given extensions, recursively")
    parser.add_argument("--extensions", help="File extensions to be formatted", required=False, default={'c', 'h', 'cpp', 'hpp'})
    parser.add_argument("--executable", help='Path to the clang-format executable', required=False, default='clang-format')
    return parser.parse_args()

def format_file(executable, file_path):
    print(f"clang-format: {file_path}")
    try:
        process_result = subprocess.run([executable, '-i', file_path, '-style=file'], check=True)
        process_result.check_returncode()
    except Exception as e:
        print(f"Error formatting {file_path} {e}")
        return 1
    return 0

def find_files(directory, extensions):
    directory_path = pathlib.Path(directory)
    for file_path in directory_path.rglob('*'):
        if file_path.suffix[1:] in extensions:
            yield file_path

def check_executable(executable):
    print(f"Using clang-format executable: {executable}")
    try:
        process_result = subprocess.run([executable, '--version'], check=True)
        process_result.check_returncode()
    except Exception as e:
        print(f"Error with clang-format executable: {executable} {e}")
        sys.exit(1)

def main():
    print(f"{APPLICATION_NAME} - Execution started")
    args = parse()
    directory = pathlib.Path(args.directory)
    extensions = args.extensions
    executable = args.executable

    check_executable(executable)

    with concurrent.futures.ThreadPoolExecutor(max_workers=sys.maxsize) as executor:
        files = list(find_files(directory, extensions))
        futures = {executor.submit(format_file, executable, file): file for file in files}
        for future in concurrent.futures.as_completed(futures):
            if future.result() != 0:
                print(f"Error running {APPLICATION_NAME} - Execution finished")
                return future.result()
    print(f"{APPLICATION_NAME} - Execution finished")
    return 0

if __name__ == '__main__':
    sys.exit(main())
