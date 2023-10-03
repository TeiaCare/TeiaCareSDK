# TeiaCoreSDK
Welcome to TeiaCoreSDK!

---

## Environment Setup
- Create a virtual environment:

```bash
python -m pip install --upgrade pip
```

```bash
# Linux
python -m venv .venv
source .venv/bin/activate
```

```bash
# Windows
python -m venv .venv
.venv\Scripts\activate.bat
```

- Install project tools
```bash
pip install -r scripts/requirements.txt --no-cache-dir --disable-pip-version-check
```

## Dependencies Setup
- Create Conan profile for the current environment and choosen compiler
```bash
python scripts/conan/profile.py <COMPILER_NAME> <COMPILER_VERSION>

# Example: clang-15 (Linux/Windows/MacOS)
python scripts/conan/profile.py clang 15
# profile created in scripts/profiles/clang15

# Example: gcc-12 (Linux/Windows/MacOS)
python scripts/conan/profile.py gcc 12
# profile created in scripts/profiles/gcc12

# Example: msvc v17 (Visual Studio 2022, Windows)
python scripts/conan/profile.py msvc 17
# profile created in scripts/profiles/msvc17

# Example: apple-clang 14 (MacOS)
python scripts/conan/profile.py apple-clang 14
# profile created in scripts/profiles/apple-clang14
```

- Install packages for the given conan profile and build configuration (Debug or Release)
```bash
python scripts/conan/install.py <Debug|Release> <PROFILE_NAME> -d sdk/tests
python scripts/conan/install.py <Debug|Release> <PROFILE_NAME> -d sdk/examples
python scripts/conan/install.py <Debug|Release> <PROFILE_NAME> -d sdk/benchmarks

# Example: Release with gcc12 profile
python scripts/conan/install.py Release gcc12

# Example: Debug with clang15 profile
python scripts/conan/install.py Debug clang15
```

## Configure Pre-Steps
- Only for Windows builds with Visual Studio compiler

_Notes:_  
- This is required to set the MSVC development environment on Windows platform using Visual Studio.  
- The following CMake steps (Configure and Build) must be executed in the same shell on which the MSVC environment has been setup using one of the following scripts (according to your Visual Studio installation).

```bash
# Visual Studio 2019 - Community
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

# Visual Studio 2022 - Professional
"C:\Program Files (x86)\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"

# Visual Studio 2022 - Build Tools
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
```

- Only for Linux/MacOS

_Notes:_  
- Set the environment variable CXX to the full path or symlink of C++ compiler used.
- The value of CXX is inherited in CMake and set to CMAKE_CXX_COMPILER.

```bash
# clang 15
export CXX=clang++-15

# gcc 12
export CXX=g++-12

# apple-clang 14
export CXX=apple-clang-14
```

## Configure
```bash
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -B ./build/Debug -S .
cmake -G Ninja -D CMAKE_BUILD_TYPE=Release -B ./build/Release -S .
```

## Build
```bash
cmake --build ./build/Debug --config Debug
cmake --build ./build/Release --config Release
```

## Install
```bash
cmake --install ./build/Debug --prefix ./install/Debug
cmake --install ./build/Release --prefix ./install/Release
```

## Unit Tests
```bash
ctest --test-dir ./build/Debug --output-on-failure --output-junit ../../results/tests.xml
ctest --test-dir ./build/Release --output-on-failure --output-junit ../../results/tests.xml
```

## Code Coverage
```bash
# gcc 12
python scripts/tools/run_gcovr.py gcc 12

# clang 15
python scripts/tools/run_gcovr.py clang 15
```

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


## Documentation (Doxygen)
```bash
apt-get install doxygen graphviz
```