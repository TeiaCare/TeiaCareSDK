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

conan remote add artifactory "$artifactory_host"/artifactory/api/conan/conan False
conan user "$artifactory_username" -r artifactory -p "$artifactory_password"
cd sdk_package_test 
python3 build.py "$build_type" "$compiler" "$compiler_version"
output=$(install/teiacare_sdk_integration_test)

echo "SDK Package Test Output: $output"

expcted_version=$(cat ../VERSION)
expcted_version="v$expcted_version"
echo "Expected version: $expcted_version"

# Split the string into words using shlex.split
IFS=' ' read -ra words <<< "$output"
current_version=${words[1]}
echo "Current version: $current_version"

# Check that the version is equal to the expected version
if [[ "$current_version" == "$expcted_version" ]]; then
  echo "Correct version, package correctly validated."
else
  echo "Incorrect version"
  exit 1
fi