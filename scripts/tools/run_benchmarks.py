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
from command import run, create_directory

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("program_path", help="Path to the program to analyze")
    parser.add_argument("--benchmarks_output_path", help="Benchmarks results output path", required=False, default='./results/benchmarks/results.json')
    return parser.parse_args()

def benchmarks(args):
    create_directory(args.benchmarks_output_path)

    cmd = [
        args.program_path,
        f'--benchmark_out={args.benchmarks_output_path}',
        '--benchmark_out_format=json',
        '--benchmark_format=console',
        '--benchmark_time_unit=ms',
        '--benchmark_repetitions=3'
    ]
    run(cmd, debug=True)

if __name__ == '__main__':
    benchmarks(parse())
