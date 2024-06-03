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
from command import run

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", help="Debug or Release", choices=['Debug', 'Release', 'RelWithDebInfo'])
    parser.add_argument("--run_gtest", required=False, default=False, action='store_true')
    parser.add_argument("--test_dir", help="Unit Tests root directory", required=False, default='./build')
    parser.add_argument("--xml_results_path", help="Unit Tests xml results path", required=False, default='../../results/unit_tests/unit_tests.xml')
    return parser.parse_args()

def run_ctest(args):
    run([
        'ctest',
        '--parallel', '1',
        '--test-dir', f'{args.test_dir}/{args.build_type}',
        '--output-junit', args.xml_results_path,
        '--timeout', '30', # seconds
        '--output-on-failure',
        '--progress',
        '--schedule_random'
    ], debug=True, check_returncode=False)

def run_gtest(args):
    run([
        args.program_path,
        '--gtest_shuffle',
        '--gtest_random_seed=0',
        '--gtest_brief=0',
        '--gtest_print_time=1',
        f'--gtest_output=xml:{args.xml_results_path}'
    ], debug=True, check_returncode=False)

if __name__ == '__main__':
    run(['ctest', '--version'])
    args = parse()

    if args.run_gtest:
        run_gtest(args)
    else:
        run_ctest(args)
