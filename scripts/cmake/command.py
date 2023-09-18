#!/usr/bin/python
import subprocess
import sys

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
