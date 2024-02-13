# TeiaCoreSDK
Welcome to TeiaCoreSDK!

*TeiaCoreSDK* is a collection of reusable C++ components.

---

## Environment Setup
- Create a virtual environment:

```bash
python -m pip install --upgrade pip
python -m venv .venv

# Linux
echo "export CONAN_USER_HOME=$PWD" >> .venv/bin/activate
source .venv/bin/activate

# Windows
echo set CONAN_USER_HOME=%CD%>>.venv\Scripts\activate.bat
.venv\Scripts\activate.bat

pip install -r scripts/requirements.txt
```

## Setup Build Environment (Windows only)
When building from command line on Windows it is necessary to activate the Visual Studio Developer Command Prompt.
Depending on the version of Visual Studio compiler and on its install location it is required to run *vcvars64.bat* script the set the development environment properly.  
*Note*: using Visual Studio IDE or the CMake extension for VSCode this step is already managed in the background.  

Examples:

```bash
# Visual Studio 2022 - Build Tools
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# Visual Studio 2019 - Enterprise
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
```

## Dependencies Setup
```bash
python scripts/conan/setup.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
```

## Configure, Build and Install
```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
python scripts/cmake/build.py <Debug|Release>
python scripts/cmake/install.py <Debug|Release>
```

## Unit Tests
```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --unit_tests --warnings
python scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
python scripts/tools/run_unit_tests.py <Debug|Release>
```
Examples are installed in $PWD/install/unit_tests.
Unit tests results are available in $PWD/results/unit_tests.  


## Code Coverage
Note that code coverage is not available on Windows.

```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --coverage --warnings
python scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
python scripts/tools/run_unit_tests.py <Debug|Release>
python scripts/tools/run_coverage.py <COMPILER_NAME> <COMPILER_VERSION>
```
Unit tests results are available in $PWD/results/unit_tests.  
Coverage results are available in $PWD/results/coverage.

## Examples
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --examples --warnings
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
```
Examples are installed in $PWD/install/examples.

## Benchmarks
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --benchmarks --warnings
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release> 
```
Benchmarks are installed in $PWD/install/benchmarks.

## Code Formatting (clang-format)
```bash
python ./scripts/tools/run_clang_format.py -r -i sdk
```

## Code Analysis (clang-tidy) 
```bash
python ./scripts/tools/run_clang_tidy.py -header-filter=.* sdk
```

## Code Analysis (cppcheck) [TODO: Review]
```bash
# TODO: add python script.
```

## Code Analysis (cpplint) [TODO: Review]
```bash
# TODO: add python script.

cpplint --counting=detailed  $(find teiacore_sdk* -type f -name "*.hpp" -or -name "*.cpp")
```

## Documentation
First install and setup doxygen from your OS package manager.
```bash
# Linux
apt-get install doxygen graphviz

# Windows
winget install doxygen

# Update Doxyfile (required only after Doxygen updates)
doxygen -u sdk/docs/Doxyfile
```

```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --docs
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
```
Documentation is now installed in $PWD/install/docs.

## Conan Package - Local Install
```bash
git clone https://teiacare@dev.azure.com/teiacare/Ancelia/_git/TeiaCoreSDK
cd TeiaCoreSDK

# Create and install local package at --install_dir path
python ./scripts/conan/create.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --install_dir <INSTALL_DIR_PATH>
```

## Conan Package - Test
```bash
git clone https://teiacare@dev.azure.com/teiacare/Ancelia/_git/TeiaCoreSDK
cd TeiaCoreSDK
cd sdk_package_test

# Build and install the test package executable at $PWD/install/teiacore_sdk_client_package_test
python build.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> 

# Run it
$PWD/install/teiacore_sdk_client_package_test
```