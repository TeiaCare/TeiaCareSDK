#!/usr/bin/python
import subprocess
import sys
import os

def run(command):
    try:
        ret = subprocess.run(command)
        ret.check_returncode()
    except subprocess.CalledProcessError as e:
        print(f'Process Exception: {e}')
        sys.exit(e.returncode)
    except Exception as e:
        print(f'Unhandled Exception: {e}')
        sys.exit(1)

def check_venv():
    venv_path = os.getenv('VIRTUAL_ENV')
    if not venv_path:
        raise SystemError("\n========================================================"
                          "\nYou are not running inside a python virtual environment"
                          "\nConfigure and activate it as shown in the project README"
                          "\n========================================================\n")
