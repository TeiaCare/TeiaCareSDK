# TeiaCareSDK
Welcome to TeiaCareSDK!


TeiaCareSDK is a collection of reusable C++ components designed to make your development journey smoother, faster, and more enjoyable. With its modular approach and extensive features, TeiaCareSDK empowers you to focus on the core functionality of your application while providing essential building blocks for common tasks.

![TeiaCareSDK](https://socialify.git.ci/TeiaCare/TeiaCareSDK/image?description=1&font=Raleway&name=1&pattern=Solid&theme=Dark)

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


## What’s included in the SDK
| **Component**        | **Description**                                                                                                                      |
|:--------------------:|--------------------------------------------------------------------------------------------------------------------------------------|
| Datetime             | Classes for date, time, and duration handling, including validation, formatting, conversions, timezones, and arithmetic operations.  |
| Blocking Queue       | Thread-safe blocking queue with fixed capacity, offering blocking push/pop and optional try_push/try_pop.                            |
| Clock                | Wrappers and utilities steady_clock, time_point and duration.                                                                        |
| Event Dispatcher     | Event dispatcher with thread pool, supporting handler registration, event emission, and removal with various argument types.         |
| Function Traits      | Template metaprogramming to extract function information and create std::function wrappers.                                          |
| High Precision Timer | High-precision timer with millisecond accuracy, offering start/stop, callback setting, and missed/invoked callback counts.           |
| Non Copiable         | Base class to prevent copying of derived classes.                                                                                    |
| Non Moveable         | Base class to prevent moving of derived classes.                                                                                     |
| Observable           | Thread-safe observer pattern implementation with callback and enable/disable control.                                                |
| Rate Limiter         | Rate limiter to synchronize tasks with a desired frame rate.                                                                         |
| Service Locator      | Type-safe, centralized service registry for managing and accessing services in an application.                                       |
| Singleton            | Template-based singleton implementation for thread-safe, lazy initialization of a single instance of any type.                       |
| Stop Watch           | Stopwatch for measuring elapsed time with generic and specific duration retrieval.                                                   |
| Task                 | Move-only, type-erased callable object wrapper for safe and flexible task execution.                                                 |
| Task Scheduler       | Versatile task scheduler for executing tasks at specific times, with delays, or periodically, offering task management capabilities. |
| Thread Pool          | Thread pool for asynchronous task execution with future-based result access and start/stop control.                                  |
| Unreachable          | Utility for signaling unexpected code paths.                                                                                         |
| UUID                 | Class for representing and working with UUID Version 4 values, including creation, comparison, and string/byte representations.      |
| UUID Generator       | Singleton for generating and managing UUID Version 4 values, offering creation from strings and random generation.                   |
| Version              | Namespace with details about project, build, and operating system.                                                                   |

## Supported Configurations
Azure DevOps build matrix is available [here](https://dev.azure.com/teiacare/TeiaCare/_build?definitionId=91&_a=summary).

| **OS**               | **Compiler**       | **Debug** | **Release** | **RelWithDebInfo** |
|----------------------|--------------------|:---------:|:-----------:|:------------------:|
| Windows Server 2022  | Visual Studio 2022 |  [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20windows2022_msvc2022)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |[![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20windows2022_msvc2022)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20windows2022_msvc2022)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Windows Server 2022  | Clang 16           | 🚧 | 🚧 | 🚧 |
| Windows Server 2019  | Visual Studio 2019 | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20windows2019_msvc2019)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20windows2019_msvc2019)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20windows2019_msvc2019)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Windows Server 2019  | Clang 16           | 🚧 | 🚧 | 🚧 |
| Ubuntu 22.04         | GCC 12             | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20ubuntu2204_gcc12)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20ubuntu2204_gcc12)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20ubuntu2204_gcc12)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Ubuntu 22.04         | Clang 15           | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20ubuntu2204_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20ubuntu2204_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20ubuntu2204_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |
| Ubuntu 24.04         | GCC 14             | 🚧 | 🚧 | 🚧 |
| Ubuntu 24.04         | Clang 18           | 🚧 | 🚧 | 🚧 |
| MacOS 14 Sonoma      | Apple-Clang 15     | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Debug&configuration=Debug%20macos14_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=Release&configuration=Release%20macos14_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) | [![Build Status](https://dev.azure.com/teiacare/TeiaCare/_apis/build/status%2FTeiaCareSDK?branchName=develop&jobName=RelWithDebInfo&configuration=RelWithDebInfo%20macos14_clang15)](https://dev.azure.com/teiacare/TeiaCare/_build/latest?definitionId=91&branchName=develop) |

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

**Create a virtual environment**

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

# on developer machine only (not in CI)
pip install pre-commit==3.7.1
pre-commit install
```

**Setup Build Environment (Windows Only)**

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

**Dependencies Setup**
```bash
python scripts/conan/setup.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
```

**Configure, Build and Install**
```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
python scripts/cmake/build.py <Debug|Release>
python scripts/cmake/install.py <Debug|Release>
```


## Examples
```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --examples --warnings
python scripts/cmake/build.py <Debug|Release>
python scripts/cmake/install.py <Debug|Release>
```
Examples are installed in $PWD/install/examples.

## Unit Tests
```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --unit_tests --warnings
python scripts/cmake/build.py <Debug|Release>
python scripts/cmake/install.py <Debug|Release>
python scripts/tools/run_unit_tests.py <Debug|Release>
```
Examples are installed in $PWD/install/unit_tests.
Unit tests results are available in $PWD/results/unit_tests.


## Code Coverage
Note that code coverage is not available on Windows.

```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --coverage --warnings
python scripts/cmake/build.py <Debug|Release>
python scripts/cmake/install.py <Debug|Release>
python scripts/tools/run_unit_tests.py <Debug|Release>
python scripts/tools/run_coverage.py <COMPILER_NAME> <COMPILER_VERSION>
```
Unit tests results are available in $PWD/results/unit_tests.
Coverage results are available in $PWD/results/coverage.

## Benchmarks
```bash
python scripts/cmake/configure.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION> --benchmarks --warnings
python scripts/cmake/build.py <Debug|Release>
python scripts/cmake/install.py <Debug|Release>
```
Benchmarks are installed in $PWD/install/benchmarks.

## Code Formatting
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

*clang-format* can be installed via *pip* using the provided *scripts/requirements.txt*

```bash
python scripts/tools/run_clang_format.py -r -i sdk
```

## Code Analysis
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)

*clang-tidy* can be installed via *pip* using the provided *scripts/requirements.txt*

```bash
python scripts/tools/run_clang_tidy.py -header-filter=.* sdk
```

- [cppcheck](http://cppcheck.net/)

First install and setup *cppcheck* from your OS package manager.
```bash
# Linux
sudo apt install cppcheck

# Windows
winget install cppcheck
```

```bash
python scripts/tools/run_cppcheck.py <Debug|Release>
```

- [cpplint](https://github.com/cpplint/cpplint) [TODO: Review]
```bash
# TODO: add python script.
cpplint --counting=detailed  $(find teiacare_sdk* -type f -name "*.hpp" -or -name "*.cpp")
```

## Generate Documentation
First install and setup *Doxygen* from your OS package manager.
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
git clone https://teiacare@dev.azure.com/teiacare/Ancelia/_git/TeiaCareSDK
cd TeiaCareSDK

# Create, test and install local package
# Notes:
# 1) The install directory path must be a valid Conan cache (i.e. ".conan" folder) located in the current directory
#    So, in order to install the package in a desired repository folder, it is required to run this script from the repository folder directly.
# 2) The Conan package tests are automatically run during package creation.
#    The directory test_package contains a test project that is built to validate the proper package creation.

python ./scripts/conan/create.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>

# Build, install and run the test package executable
python test_package/build.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
$PWD/install/test_package/teiacare_sdk_test_package
```

## Conan Package - Artifactory Setup
In order to push a Conan package to TeiaCare artifactory server it is required to setup you local Conan client with the following commands:

```bash
# export CONAN_REVISIONS_ENABLED=1
conan remote add artifactory http://<ADDRESS>:<PORT>/artifactory/api/conan/conan
conan user -p <ARTIFACTORY_PERSONAL_ACCESS_TOKEN> -r artifactory <YOUR_USERNAME>

python3 scripts/conan/create.py <Debug|Release> <COMPILER_NAME> <COMPILER_VERSION>
python scripts/conan/upload.py artifactory teiacare_sdk
```

## Contributing
In order to contribute to TeiaCareSDK, please follow our [contribution guidelines](./CONTRIBUTING).

[![Contributions](https://img.shields.io/badge/Contributions-Welcome-green.svg)](./CONTRIBUTING)

## License
This project is licensed under the [Apache License, Version 2.0](./LICENSE).
Copyright © 2024 [TeiaCare](https://teiacare.com/)

[![License](https://img.shields.io/badge/License-Apache_v2-blue)](./LICENSE)
