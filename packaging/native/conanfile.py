from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, cmake_layout
class NativeSDK(ConanFile):
    name = "interceptor-gamepad-controller-sdk"
    version = "1.0.13"
    license = "MIT"
    description = "Public Windows x64 user-mode SDK for Interceptor-Gamepad-Controller; driver not included."
    settings = "os", "arch", "compiler", "build_type"
    exports_sources = "CMakeLists.txt", "*.cmake.in", "sdk/*", "LICENSE", "licenses/*", "NOTICE*", "README.txt"
    generators = "CMakeToolchain", "CMakeDeps"
    def validate(self):
        if str(self.settings.os) != "Windows" or str(self.settings.arch) != "x86_64":
            raise ConanInvalidConfiguration("Windows x64 is required")
    def layout(self):
        cmake_layout(self)
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    def package(self):
        CMake(self).install()
    def package_info(self):
        self.cpp_info.libs = ["TitanInterceptorGamepadController"]
        self.cpp_info.includedirs = ['include', 'include/Interceptor']
        self.cpp_info.system_libs = ['advapi32', 'bcrypt', 'cfgmgr32', 'comctl32', 'hid', 'newdev', 'ole32', 'setupapi', 'user32']
        self.cpp_info.set_property("cmake_file_name", "InterceptorGamepadControllerSDK")
        self.cpp_info.set_property("cmake_target_name", "InterceptorGamepadControllerSDK::sdk")
