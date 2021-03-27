# ------------------------------------------------------------
# This file is part of mem of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020-2021 headcode.space e.U.
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

from conans import ConanFile, CMake, tools


class HcsUrlConan(ConanFile):
    """Conan package manager file. See https://conan.io"""
    name = "@CMAKE_PROJECT_NAME@"
    version = "@VERSION@"
    license = "MIT"
    author = "Oliver Maurhart (oliver.maurhart@headcode.space)"
    url = "https://gitlab.com/headcode.space/url"
    homepage = "https://gitlab.com/headcode.space/url"
    description = "A small C++17 library parsing and resolving URLs (and URIs)."
    topics = ("url", "parser", "small", "self-contained", "C++17")
    no_copy_source = True
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        git_source = "@CMAKE_SOURCE_DIR@" or "https://gitlab.com/headcode.space/url.git"
        self.run(f"git clone --recurse-submodules {git_source} {self.name}")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="hcs-url")
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="include", src=f"{self.name}/include")
        self.copy("*.h", dst="include", src=f"{self.name}/include")
        self.copy("*hcs-url.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["libhcs-url"]
        self.cpp_info.includedirs = ["include/headcode/url"]
