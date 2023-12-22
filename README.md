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
echo setx CONAN_USER_HOME "%CD%" >> .venv\Scripts\activate.bat
.venv\Scripts\activate.bat

pip install -r scripts/requirements.txt
```

## Dependencies Setup
```bash
python scripts/conan/setup.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
```

## Configure Pre-Steps
- Only for Windows builds with Visual Studio compiler
```bash
# TODO
```

_Notes:_  
- This is required to set the MSVC development environment on Windows platform using Visual Studio.  
- The following CMake steps (Configure and Build) must be executed in the same shell on which the MSVC environment has been setup using one of the following scripts (according to your Visual Studio installation).

## Configure, Build and Install
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
python ./scripts/cmake/build.py <Debug|Release>
python ./scripts/cmake/install.py <Debug|Release>
```

## Unit Tests
```bash
ctest --test-dir ./build/<Debug|Release> --output-on-failure --output-junit ../../results/tests.xml
```

## Code Coverage
```bash
python scripts/tools/run_gcovr.py <COMPILER_NAME> <COMPILER_VERSION>
```

## Examples
```bash
python ./scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --examples
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

## Code Formatting (clang-format)
```bash
python ./scripts/tools/run_clang_format.py . -r -i

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_ENABLE_CLANG_FORMAT=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_clang_format
```

## Code Analysis (clang-tidy)
```bash
python ./scripts/tools/run_clang_tidy.py .

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_ENABLE_CLANG_TIDY=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_clang_tidy
```

## Code Analysis (cppcheck)
```bash
# TODO: add python script.

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_ENABLE_CPPCHECK=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_cppcheck
```

## Code Analysis (cpplint)
```bash
# TODO: replace with a python script.
cpplint --counting=detailed  $(find teiacore_sdk* -type f -name "*.hpp" -or -name "*.cpp")

# From CMake:
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D TEIACORE_ENABLE_CPPLINT=True -B ./build/Debug -S .
cmake --build ./build/Debug --target teiacore_sdk_cpplint
```

## Documentation
```bash
apt-get install doxygen graphviz

# TODO
```