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

import argparse
import pathlib
from command import run

def check():
    run(['cppcheck', '--version'])

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release', 'RelWithDebInfo'])
    return parser.parse_args()

def cppcheck(args):
    cwd = pathlib.Path().resolve()
    build_dir = pathlib.Path(cwd, f'build/{args.build_type}').absolute()    
    compile_commands_path = pathlib.Path(build_dir, 'compile_commands.json').absolute()
    cppcheck_dir = pathlib.Path(build_dir, 'cppcheck').absolute()
    cppcheck_dir.mkdir(parents=True, exist_ok=True)
    
    run([
        "cppcheck",
        # "--enable=all",
        "--error-exitcode=1",
        f'--project={compile_commands_path}',
        f'--cppcheck-build-dir={cppcheck_dir}',
        # Exclude Google Test macros
        "-DTEST", "-DTEST_F", "-DTEST_P", "-DTYPED_TEST",
        # Exclude 3rd party library date.h
        f'--suppress=*:{cwd}/sdk/include/teiacare/sdk/datetime/date/*', 
        # The one definition rule is violated, different classes/structs have the same name 'Foo'
        f'--suppress=ctuOneDefinitionRuleViolation:{cwd}/sdk/examples/*' 
    ], check_returncode=True)

if __name__ == '__main__':
    check()
    # args=argparse.Namespace()
    # args.build_type='Debug'
    args=parse()
    cppcheck(args)
