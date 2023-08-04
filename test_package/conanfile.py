#!/usr/bin/env python
import os
from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import cross_building

class TeiaCoreSDKTest(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if not cross_building(self):
            cmd = os.path.join(self.cpp.build.bindirs[0], "teiacore_sdk_package_test")
            self.run(cmd, env="conanrun")

# TODO: create wrapper script
# conan test test_package --profile:build ./scripts/profiles/gcc12 --profile:host ./scripts/profiles/gcc12 teiacore_sdk/1.0.0