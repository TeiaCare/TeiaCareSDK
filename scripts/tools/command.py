#!/usr/bin/python
import sys
import subprocess
import pathlib

def run(command, env_vars={}, debug=False, check_returncode=True):
    if debug:
        print(command)
    try:
        ret = subprocess.run(command, env=env_vars)
        if check_returncode:
            ret.check_returncode()
    except subprocess.CalledProcessError as e:
        print(f'Process Exception: {e}')
        sys.exit(e.returncode)
    except Exception as e:
        print(f'Unhandled Exception: {e}')
        sys.exit(1)

def create_directory(file_path):
    path = pathlib.Path(file_path)
    dir = path.absolute().parent
    dir.mkdir(parents=True, exist_ok=True)