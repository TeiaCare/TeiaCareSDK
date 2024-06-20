import sys
import concurrent.futures
import subprocess
import pathlib
import argparse

APPLICATION_NAME = "clang-tidy runner"

def parse():
    parser = argparse.ArgumentParser(APPLICATION_NAME, formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("directory", help="Directory to look for files, of the given extensions, recursively")
    parser.add_argument("--extensions", help="File extensions to be validated", required=False, default={'c', 'h', 'cpp', 'hpp'})
    parser.add_argument("--executable", help='Path to the clang-tidy executable', required=False, default='clang-tidy')
    return parser.parse_args()

def tidy_file(executable, file_path):
    print(f"clang-tidy: {file_path}")
    try:
        process_result = subprocess.run([executable, file_path, "-checks=clang-analyzer-cplusplus.*"], check=True)
        process_result.check_returncode()
    except Exception as e:
        print(f"Error tidying {file_path} {e}")
        return 1
    return 0

def find_files(directory, extensions):
    directory_path = pathlib.Path(directory)
    for file_path in directory_path.rglob('*'):
        if file_path.suffix[1:] in extensions:
            yield file_path

def check_executable(executable):
    print(f"Using clang-tidy executable: {executable}")
    try:
        process_result = subprocess.run([executable, '--version'], check=True)
        process_result.check_returncode()
    except Exception as e:
        print(f"Error with clang-tidy executable: {executable} {e}")
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
        futures = {executor.submit(tidy_file, executable, file): file for file in files}
        for future in concurrent.futures.as_completed(futures):
            if future.result() != 0:
                print(f"Error running {APPLICATION_NAME} - Execution finished")
                return future.result()
    print(f"{APPLICATION_NAME} - Execution finished")
    return 0

if __name__ == '__main__':
    sys.exit(main())
