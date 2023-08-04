#!/usr/bin/env python
from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain

def get_project_version():
    version = "dev"
    try:
        with open('VERSION') as version_file:
            version = version_file.read().strip()
    except FileNotFoundError:
        print('Unable to find VERSION file in the current directory.')

    return version

class TeiaCoreSDK(ConanFile):
    name = "teiacore_sdk"
    version = get_project_version()
    license = "" # TODO: add this
    author = "TeiaCare"
    url = "https://teiacare.com/"
    description = "" # TODO: add this
    topics = ("sdk")
    exports_sources = "CMakeLists.txt", "VERSION", "sdk/*"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = "ON" if self.options.shared else "OFF"
        tc.variables["TEIACORE_ENABLE_INTEGRATION_TESTS"] = False
        tc.variables["TEIACORE_ENABLE_UNIT_TESTS"] = False
        tc.variables["TEIACORE_ENABLE_BENCHMARKS"] = False
        tc.variables["TEIACORE_ENABLE_EXAMPLES"] = False
        tc.variables["TEIACORE_ENABLE_WARNINGS_ERROR"] = False
        tc.variables["TEIACORE_ENABLE_SANITIZER_ADDRESS"] = False
        tc.variables["TEIACORE_ENABLE_SANITIZER_THREAD"] = False
        tc.variables["TEIACORE_ENABLE_CLANG_FORMAT"] = False
        tc.variables["TEIACORE_ENABLE_CLANG_TIDY"] = False
        tc.variables["TEIACORE_ENABLE_CPPCHECK"] = False
        tc.variables["TEIACORE_ENABLE_CPPLINT"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy(pattern="VERSION")
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["teiacore_sdk"]

# TODO: create a wrapper script for the conan create command

# In order to install the package in the local cache also run:
# export CONAN_USER_HOME $PWD

# conan create . _/_ --profile:build ./scripts/profiles/gcc12 --profile:host ./scripts/profiles/gcc12 -s build_type=Release
# conan create . _/_ --profile:build ./scripts/profiles/clang15 --profile:host ./scripts/profiles/clang15 -s build_type=Release