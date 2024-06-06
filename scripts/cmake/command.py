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

import subprocess
import sys
import os

def run(cmd):
    try:
        ret = subprocess.run(cmd)
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
