#!/bin/bash
set -e

# Check the number of parameters
if [ $# -ne 6 ]; then
  echo "Error: Invalid number of parameters."
  echo "Usage: $0 <build_type> <compiler> <compiler_version> <artifactory_host> <artifactory_username> <artifactory_password>"
  exit 1
fi

# Assign parameters to variables
build_type="$1"
compiler="$2"
compiler_version="$3"
artifactory_host="$4"
artifactory_username="$5"
artifactory_password="$6"

# Rest of your script...

python3 scripts/conan/create.py "$build_type" "$compiler" "$compiler_version" --install_dir .
conan remote add artifactory "$artifactory_host"/artifactory/api/conan/conan False
conan user "$artifactory_username" -r artifactory -p "$artifactory_password"
python3 scripts/conan/upload.py artifactory