# import subprocess
# subprocess.run(["ctest", "--version"])
from command import run

if __name__ == '__main__':
    run(["ctest", "--version"])
    run(['ctest', '--version'])