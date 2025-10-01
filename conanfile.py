from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class BasicConanfile(ConanFile):
    name = "theater"
    version = "0.1"
    description = "A basic recipe"
    license = "MIT"
    package_type = "application"

    settings = "os", "arch", "compiler", "build_type"
    exports_sources = "CMakeLists.txt", "src/*"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("strong_type/v15")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.24 <4]")

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.variables["CMAKE_CXX_STANDARD"] = "17"
        toolchain.variables["CMAKE_CXX_EXTENSIONS"] = "OFF"
        toolchain.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        toolchain.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def validate(self):
        cppstd = self.settings.get_safe("compiler.cppstd")
        if cppstd:
            check_min_cppstd(self, 17)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
