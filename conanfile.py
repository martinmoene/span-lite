from conans import ConanFile

class SpanLiteConan(ConanFile):
    version = "0.4.0"
    name = "span-lite"
    description = "A C++20-like span for C++98, C++11 and later in a single-file header-only library"
    license = "Boost Software License - Version 1.0. http://www.boost.org/LICENSE_1_0.txt"
    url = "https://github.com/martinmoene/span-lite.git"
    exports_sources = "include/nonstd/*", "LICENSE.txt"
    build_policy = "missing"    
    author = "Martin Moene"

    def build(self):
        """Avoid warning on build step"""
        pass

    def package(self):
        """Provide pkg/include/nonstd/*.hpp"""
        self.copy("*.hpp")

    def package_info(self):
        self.info.header_only()
