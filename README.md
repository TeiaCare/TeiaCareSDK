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
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
```

## Unit Tests [TODO: Review] 
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --tests --warnings
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
```
Unit tests are now installed in $PWD/install/tests.

Run unit tests with:
```bash
# Run from build directory (does not require previous cmake install step)
ctest --test-dir ./build/<Debug|Release> --output-on-failure --output-junit ../../results/tests.xml

# Run from install directory (requires cmake install step)
ctest --test-dir ./install/tests --output-on-failure --output-junit ../../results/tests.xml
```

## Code Coverage [TODO: Review] 
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --coverage --warnings
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
```
Unit tests (built with code coverage enabled) are now installed in $PWD/install/tests.

Run unit tests and generate coverage report with:
```bash
python scripts/tools/run_coverage.py <COMPILER_NAME> <COMPILER_VERSION>
```

## Examples
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --examples --warnings
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
```
Examples are now installed in $PWD/install/examples.

## Benchmarks
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --benchmarks --warnings
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release> 
```
Benchmarks are now installed in $PWD/install/benchmarks.

## Code Formatting (clang-format) [TODO: Review]
```bash
python ./scripts/tools/run_clang_format.py . -r -i

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_SDK_ENABLE_CLANG_FORMAT=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_clang_format
```

## Code Analysis (clang-tidy) [TODO: Review] 
```bash
python ./scripts/tools/run_clang_tidy.py .

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_SDK_ENABLE_CLANG_TIDY=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_clang_tidy
```

## Code Analysis (cppcheck) [TODO: Review]
```bash
# TODO: add python script.

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_SDK_ENABLE_CPPCHECK=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_cppcheck
```

## Code Analysis (cpplint) [TODO: Review]
```bash
# TODO: replace with a python script.
cpplint --counting=detailed  $(find teiacore_sdk* -type f -name "*.hpp" -or -name "*.cpp")

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_SDK_ENABLE_CPPLINT=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_cpplint
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