import sys
from concurrent.futures import ThreadPoolExecutor

import concurrent.futures
import os
import subprocess
import time
import pathlib
import argparse

APPLICATION_NAME = "clang-format runner"

def parse():
    parser = argparse.ArgumentParser(APPLICATION_NAME, formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("directory", help="Directory to look for files, of the given extensions, recursively")
    parser.add_argument("--extensions", help="File extensions to be formatted", required=False, default={'c', 'h', 'cpp', 'hpp'})
    return parser.parse_args()

def format_file(file_path):
    print(f"Formatting {file_path}")
    try:
        process_result = subprocess.run(['clang-format', '-i', file_path], check=True)
        process_result.check_returncode()
    except Exception as e:
        print(f"Error formatting {file_path} {e}")

    return process_result.returncode

def find_files(directory, extensions):
    directory_path = pathlib.Path(directory)
    for file_path in directory_path.rglob('*'):
        if file_path.suffix[1:] in extensions:
            yield file_path

def main():
    print(f"{APPLICATION_NAME} - Execution started")

    args = parse()
    directory = pathlib.Path(args.directory)
    extensions = args.extensions

    with concurrent.futures.ThreadPoolExecutor(max_workers=sys.maxsize) as executor:
        files_to_format = list(find_files(directory, extensions))
        futures = {executor.submit(format_file, file): file for file in files_to_format}

        for future in concurrent.futures.as_completed(futures):
            if future.result() != 0:
                print(f"Error running {APPLICATION_NAME} - Execution finished")
                return future.result()

    print(f"{APPLICATION_NAME} - Execution finished")
    return 0

if __name__ == '__main__':
    sys.exit(main())
