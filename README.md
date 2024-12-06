# TeiaCareSDK

Welcome to TeiaCareSDK!

TeiaCareSDK is a collection of reusable C++ components.

[![Docs](https://img.shields.io/badge/Docs-TeiaCareSDK-white.svg)](https://teiacare.github.io/TeiaCareSDK/)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/ba48b0836d884c969b94b314901b95ff)](https://app.codacy.com/gh/TeiaCare/TeiaCareSDK/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Codacy Badge](https://app.codacy.com/project/badge/Coverage/ba48b0836d884c969b94b314901b95ff)](https://app.codacy.com/gh/TeiaCare/TeiaCareSDK/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_coverage)
[![codecov](https://codecov.io/gh/TeiaCare/TeiaCareSDK/branch/develop/graph/badge.svg?token=EFEW3J454E)](https://codecov.io/gh/TeiaCare/TeiaCareSDK)

![TeiaCareSDK](https://socialify.git.ci/TeiaCare/TeiaCareSDK/image?description=1&font=Raleway&name=1&pattern=Solid&theme=Auto)

## Key Features

* **Date & Time:** Master dates, times, and durations with ease using dedicated classes for manipulation, validation, formatting, conversions, timezones, and arithmetic operations.
* **Concurrency:** Take control of concurrent execution with thread-safe modules for managing tasks, queues, events, and thread pools, enabling efficient parallel processing.
* **Synchronization & Control:** Maintain data integrity and program flow with modules for rate limiting, observing data changes, and accurately measuring elapsed time, ensuring thread safety and coordinated execution.
* **Platform Agnostic:** Benefit from cross-platform compatibility with modules that adapt to your system's specifics, like version information, guaranteeing seamless code execution across different environments.
* **Singletons & Services:** Create and manage global instances effectively with a dedicated module, and organize your application logic with a service locator, achieving centralized access to essential services and a clean code structure.
* **Enhanced Code Safety:** Enforce best practices with base classes that prevent copying and moving objects, use a module to signal unexpected code paths, and leverage metaprogramming techniques for function analysis in unit testing, promoting robust and secure code.
* **Unique Identifier Generation:** Easily generate and manage Universally Unique Identifiers (UUIDs) with dedicated classes, allowing for effortless creation and tracking of unique identifiers for your entities.
* **Additional Utilities:** Discover more functionality with various modules like tasks, clocks, and observers, providing a broad spectrum of tools to address diverse development needs and enhance your applications.


## Why Choose TeiaCareSDK?

* **Modular Design:** Pick and choose the modules you need, keeping your project lean and focused.
* **Clear Documentation:** Comprehensive documentation with examples ensures smooth onboarding and understanding.
* **Active Community:** Engage with our community for support, feedback, and collaboration.
* **Tested & Reliable:** Enjoy peace of mind with well-tested and actively maintained code with mature CI/CD workflows.
* **Production Quality:** TeiaCareSDK runs in production environment serving thousands of devices on a daily basis.


## Supported Configurations

Azure DevOps build matrix is available [here](https://dev.azure.com/teiacare/TeiaCare/_build?definitionId=91&_a=summary).

| **OS**               | **Compiler**       | **Debug** | **Release** | **RelWithDebInfo** |
|----------------------|--------------------|:---------:|:-----------:|:------------------:|
| Windows Server 2022  | Visual Studio 2022 |  [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20windows2022_msvc2022)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |[![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20windows2022_msvc2022)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20windows2022_msvc2022)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Windows Server 2019  | Visual Studio 2019 | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20windows2019_msvc2019)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20windows2019_msvc2019)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20windows2019_msvc2019)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| MacOS 14 Sonoma      | Apple-Clang 15     | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20macos14_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20macos14_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20macos14_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Ubuntu 22.04         | GCC 12             | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20ubuntu2204_gcc12)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20ubuntu2204_gcc12)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20ubuntu2204_gcc12)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Ubuntu 22.04         | Clang 15           | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20ubuntu2204_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20ubuntu2204_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20ubuntu2204_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Ubuntu 24.04         | GCC 14             | 🚧 | 🚧 | 🚧 |
| Ubuntu 24.04         | Clang 18           | 🚧 | 🚧 | 🚧 |


## Try online on Wandbox

[![Try Online](https://img.shields.io/badge/Wandbox-ok?style=plastic&label=Try%20Online&link=https%3A%2F%2Fimg.shields.io%2Fwandbox.org%2Fpermlink%2FsfU3VY4HFMBZp8QI)](https://wandbox.org/permlink/sfU3VY4HFMBZp8QI)

```cpp
#include <iostream>
#include "teiacare/sdk/observable.hpp"

int main()
{
    auto callback = [](int value){ std::cout << "Update: " << value << "\n"; };
    auto observable = tc::sdk::observable<int>(-1, callback);

    constexpr int total_updates = 10;
    for (int i = 0; i < total_updates; ++i)
    {
        observable = i; // Trigger updates
    }

    return 0;
}
```

Check the [Examples](sdk/examples/src/) folder for an in-depth showcase of all the features of this library.


## Getting Started

This project uses git submodules so it is required to clone it using the *--recurse-submodules* flag in order to retrive the required submodules.

```bash
git clone --recurse-submodules https://github.com/TeiaCare/TeiaCareSDK.git
```

### Create Development Environment
In order to setup a development environment it is sufficient to run the script *scripts/env/setup.<bat|sh>* depending on your operating system.

```bash
# Linux/MacOS
chmod +x scripts/env/setup.sh
scripts/env/setup.sh

# Windows
scripts\env\setup.bat
```

### Start Development Environment
In order to start the development environment it is sufficient to activate the Python Virtual Environment just created the step above.

```bash
# Linux/MacOS
source .venv/bin/activate

# Windows
.venv\Scripts\activate.bat
```

### Setup Build Environment (Windows Only)

When building from command line on Windows it is necessary to activate the Visual Studio Developer Command Prompt.

Depending on the version of Visual Studio compiler and on its install location it is required to run *vcvars64.bat* script the set the development environment properly.

*Note*: using Visual Studio IDE or the CMake extension for VSCode this step is already managed in the background, so no action is required.

Examples:

```bash
# Visual Studio 2022 - Build Tools
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# Visual Studio 2019 - Enterprise
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
```

### Setup Dependencies
This script must be executed in order to install the 3rd party dependencies specified in the conanfile (note that 3rd party libs are only required for unit tests, examples and benchmarks).

See the section [Scripts Parameters](#scripts-parameters) for more details on the allowed scripts parameters.

```bash
python scripts/conan/setup.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>

# examples:
python scripts/conan/setup.py Debug visual_studio 17
python scripts/conan/setup.py Release clang 15
```

### Configure, Build and Install

This script configures, builds and installs the library.
```bash
python scripts/cmake.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>
```

## Examples

```bash
# Build all the examples
python scripts/cmake.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION> --examples --warnings

# Run all the examples
python3 scripts/tools/run_examples.py install/examples
```
Examples are installed in $PWD/install/examples.


## Unit Tests and Code Coverage

```bash
# Build Unit Tests with Code Coverage enabled (if supported)
python scripts/cmake.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION> --coverage --warnings

# Run Unit Tests
python scripts/tools/run_unit_tests.py <Debug|Release|RelWithDebInfo>

# Run Code Covergae
python scripts/tools/run_coverage.py <COMPILER_NAME> <COMPILER_VERSION> sdk --extra_args=-e=sdk/src/datetime/date.h
```
Note that code coverage is not available on Windows.

Unit tests results are available in $PWD/results/unit_tests.
Coverage results are available in $PWD/results/coverage.


## Benchmarks

```bash
python scripts/cmake.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION> --benchmarks --warnings
python scripts/tools/run_benchmarks.py <COMPILER_NAME> <COMPILER_VERSION>
```
Benchmarks are installed in $PWD/install/benchmarks.


## Code Formatting

[clang-format](https://clang.llvm.org/docs/ClangFormat.html)

*clang-format* can be installed via *pip* using the provided *scripts/requirements.txt*

```bash
python scripts/tools/run_clang_format.py sdk
```


## Code Analysis

[clang-tidy](https://clang.llvm.org/extra/clang-tidy/)

*clang-tidy* can be installed via *pip* using the provided *scripts/requirements.txt*

```bash
python scripts/tools/run_clang_tidy.py -header-filter=.* sdk
```

[cppcheck](http://cppcheck.net/)

First install and setup *cppcheck* from your OS package manager.
```bash
# Linux
sudo apt install cppcheck

# Windows
winget install cppcheck
```

Then run CppCheck using the provided python script:
```bash
python scripts/tools/run_cppcheck.py <Debug|Release|RelWithDebInfo>
```


## Generate Documentation

First install and setup *Doxygen* from your OS package manager.
```bash
# Linux
apt-get install doxygen graphviz

# Windows
winget install doxygen
```

Then run Doxygen using the provided python script:
```bash
python scripts/tools/run_doxygen.py
```
Documentation is now installed in $PWD/docs.

## Conan Package

### Local Install

Create, test and install local package.

Notes:

1) The install directory path must be a valid Conan cache (i.e. ".conan" folder) located in the current directory.
   So, in order to install the package in a desired repository folder, it is required to run this script from the repository folder directly.
2) The Conan package tests are automatically run during package creation.
   The directory test_package contains a test project that is built to validate the proper package creation.

```bash
# Create the Conan package locally
python scripts/conan/create.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>

# Build and install the test package executable
python test_package/build.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>

# Run the test package executable
$PWD/install/test_package/teiacare_sdk_test_package
```


### Artifactory Upload

In order to upload a Conan package to TeiaCare Artifactory server it is required to setup you local Conan client once with the following commands:

```bash
# Add TeiaCare Artifactory remote to local Conan client
conan remote add teiacare $(artifactory.url)/teiacare

# Authenticate with Artifactory credentials
conan user $(artifactory.username) -p $(artifactory.password) -r teiacare
```

Now it is possible to create and upload a Conan package with the following commands:

```bash
# Create the Conan package locally
python scripts/conan/create.py <Debug|Release|RelWithDebInfo> <COMPILER_NAME> <COMPILER_VERSION>

# Upload the package to Artifactory on the teicare remote
python scripts/conan/upload.py teiacare teiacare_sdk/<PACKAGE_VERSION>@
```


## Contributing

In order to contribute to TeiaCareSDK, please follow our [contribution guidelines](./CONTRIBUTING).

[![Contributions](https://img.shields.io/badge/Contributions-Welcome-green.svg)](./CONTRIBUTING)


## License

This project is licensed under the [Apache License, Version 2.0](./LICENSE).

[![License](https://img.shields.io/badge/License-Apache_v2-blue)](./LICENSE)

Copyright © 2024 [TeiaCare](https://teiacare.com/)
