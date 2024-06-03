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
import sys
from command import run, check_venv

def parse():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("build_type", choices=['Debug', 'Release', 'RelWithDebInfo'])
    parser.add_argument("--build_dir", help="Build Directory", required=False, default='./build')
    parser.add_argument("--install_dir", help="Install Directory", required=False, default='./install')
    return parser.parse_args()

def main():
    check_venv()
    args = parse()

    run([
        'cmake',
        '--install', f'{args.build_dir}/{args.build_type}',
        '--prefix', args.install_dir
    ])

if __name__ == '__main__':
    sys.exit(main())
