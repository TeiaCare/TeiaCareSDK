#!/usr/bin/python
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
