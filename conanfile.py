#!/usr/bin/env python
from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain

def get_project_version():
    with open('VERSION') as version_file:
        # TODO: validate Regex format
        return version_file.read().strip()

class TeiaCoreSDK(ConanFile):
    name = "teiacore_sdk"
    version = get_project_version()
    license = "" # TODO: add this
    author = "TeiaCare"
    url = "https://dev.azure.com/teiacare/Ancelia/_git/TeiaCoreSDK"
    description = "TeiaCoreSDK is a collection of reusable C++ components"
    topics = ("sdk")
    exports = "VERSION"
    exports_sources = "CMakeLists.txt", "README.md", "VERSION", "sdk/*", "cmake/*"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "CMakeDeps"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = "ON" if self.options.shared else "OFF"
        tc.variables["TC_ENABLE_UNIT_TESTS"] = False
        tc.variables["TC_ENABLE_UNIT_TESTS_COVERAGE"] = False
        tc.variables["TC_ENABLE_BENCHMARKS"] = False
        tc.variables["TC_ENABLE_EXAMPLES"] = False
        tc.variables["TC_ENABLE_DOCS"] = False
        tc.variables["TC_ENABLE_WARNINGS_ERROR"] = True
        tc.variables["TC_ENABLE_SANITIZER_ADDRESS"] = False
        tc.variables["TC_ENABLE_SANITIZER_THREAD"] = False
        tc.variables["TC_ENABLE_CLANG_FORMAT"] = False
        tc.variables["TC_ENABLE_CLANG_TIDY"] = False
        tc.variables["TC_ENABLE_CPPCHECK"] = False
        tc.variables["TC_ENABLE_CPPLINT"] = False
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
        self.cpp_info.set_property("cmake_file_name", "teiacore_sdk")
        self.cpp_info.set_property("cmake_target_name", "teiacore::sdk")
