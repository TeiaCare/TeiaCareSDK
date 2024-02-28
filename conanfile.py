#!/usr/bin/env python
# Copyright 2024 TeiaCare
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain

def get_project_version():
    with open('VERSION') as version_file:
        # TODO: validate Regex format
        return version_file.read().strip()

class TeiaCareSDK(ConanFile):
    name = "teiacare_sdk"
    version = get_project_version()
    license = "" # TODO: add this
    author = "TeiaCare"
    url = "https://dev.azure.com/teiacare/Ancelia/_git/TeiaCareSDK"
    description = "TeiaCareSDK is a collection of reusable C++ components"
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
        self.cpp_info.libs = ["teiacare_sdk"]
        self.cpp_info.set_property("cmake_file_name", "teiacare_sdk")
        self.cpp_info.set_property("cmake_target_name", "teiacare::sdk")
