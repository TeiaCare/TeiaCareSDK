# SDK Integration Test

This is an example project that shows how to consume TeiaCoreSDK as a Conan Package into a CMake-based project.

For convenience a *build.py* script can be used to perform the following:
- *install* TeiaCoreSDK Conan package as an external dependency
- *configure* the project using CMake
- *build* the project using CMake
- *install* the project using CMake

Not that the build scripts requires two arguments:
- build type (i.e. Debug or Release configuration)
- a Conan profile (must be available into *sdk_integration_test/profiles* directory)

```bash
cd sdk_integration_test
python build.py <Debug|Release> <CONAN_PROFILE_NAME>

# Example: Release, with Clang-15
python build.py Release clang15
```

