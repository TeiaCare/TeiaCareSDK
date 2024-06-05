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
import subprocess
import pathlib

def run(command, env_vars=None, debug=False, check_returncode=True):
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
