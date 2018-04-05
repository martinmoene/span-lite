span lite - A single-file header-only version of a C++20-like span for C++98, C++11 and later
====================================================
[![Standard](https://img.shields.io/badge/c%2B%2B-98/11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT) [![Build Status](https://travis-ci.org/martinmoene/span-lite.svg?branch=master)](https://travis-ci.org/martinmoene/span-lite) [![Build status](https://ci.appveyor.com/api/projects/status/1ha3wnxtam547m8p?svg=true)](https://ci.appveyor.com/project/martinmoene/span-lite) [![Version](https://badge.fury.io/gh/martinmoene%2Fspan-lite.svg)](https://github.com/martinmoene/span-lite/releases) [![download](https://img.shields.io/badge/latest-download-blue.svg)](https://github.com/martinmoene/span-lite/blob/master/include/nonstd/span.hpp) [![Try it on wandbox](https://img.shields.io/badge/on-wandbox-blue.svg)](https://wandbox.org/) [![Try it on godbolt online](https://img.shields.io/badge/on-godbolt-blue.svg)](http://bit.ly/mm-span-lite-godbolt)

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [License](#license)
- [Dependencies](#dependencies)
- [Installation and use](#installation-and-use)
- [Synopsis](#synopsis)
- [Reported to work with](#reported-to-work-with)
- [Building the tests](#building-the-tests)
- [Other implementations of span](#other-implementations-of-span)
- [Notes and references](#notes-and-references)
- [Appendix](#appendix)


Example usage
-------------

```C++
#include "nonstd/span.hpp"
#include <array>
#include <vector>
#include <iostream>

std::ptrdiff_t size( nonstd::span<int> spn )
{
    return spn.size();
}

int main()
{
    int arr[] = { 1, };
    
    std::cout << 
        "C-array:" << size( arr ) <<
        " array:"  << size( std::array <int, 2>{ 1, 2, } ) <<
        " vector:" << size( std::vector<int   >{ 1, 2, 3, } );
}
```

### Compile and run
```
prompt> g++ -std=c++11 -Wall -I../include/ -o 01-basic.exe 01-basic.cpp && 01-basic.exe
C-array:1 array:2 vector:3
```

In a nutshell
---------------
**span lite** is a single-file header-only library to provide a bounds-safe view for sequences of objects. The library provides a [C++20-like span](http://en.cppreference.com/w/cpp/container/span) for use with C++98 and later. If available, `std::span` is used, unless [configured otherwise](#configuration).

**Features and properties of span lite** are ease of installation (single header), freedom of dependencies other than the standard library. To compensate for the class template argument deduction that is missing from pre-C++17 compilers, `nonstd::span` can provide `make_span` functions. See [configuration](#configuration).


License
-------
*span lite* uses the [MIT](LICENSE) license.
 

Dependencies
------------
*span lite* has no other dependencies than the [C++ standard library](http://en.cppreference.com/w/cpp/header).


Installation and use
--------------------

*span lite* is a single-file header-only library. Put `span.hpp` in the [include](include) folder directly into the project source tree or somewhere reachable from your project.


Synopsis
--------

**Contents**  
[Documentation of `std::span`](#stdstring_view)  
[Non-standard extensions](#non-standard-extensions)  
[Configuration](#configuration)    

Documentation of `std::span`
----------------------------
Depending on the compiler and C++-standard used, `nonstd::span` behaves less or more like `std::span`. To get an idea of the capabilities of `nonstd::span` with your configuration, look at the output of the [tests](test/span.t.cpp), issuing `span-lite.t --pass @`. For `std::span`, see its [documentation at cppreference](http://en.cppreference.com/w/cpp/container/span).  

Non-standard extensions
-----------------------
### Construct from container

To construct a span from a container with compilers that cannot constrain such a single-parameter constructor to containers, *span lite* provides a constructor that takes an additional parameter of type `with_container_t`. Use `with_container` as value for this parameter.
 
### `make_span`

*span lite* can provide `make_span` creator functions to compensate for the class template argument deduction that is missing from pre-C++17 compilers. See the table below and section [configuration](#configuration).

| Kind               | std  | Function or method |                                       
|--------------------|------|--------------------|
| **Methods**        |&nbsp;| template&lt;class Container><br>constexpr **span**(with_container_t, Container & cont) |
| &nbsp;             |&nbsp;| template&lt;class Container><br>constexpr **span**(with_container_t, Container const & cont) |
| &nbsp;             |&nbsp;| &nbsp; |
| **Free functions** |&nbsp;| macro **`span_CONFIG_PROVIDE_MAKE_SPAN`** |
| &nbsp; | &nbsp;   | template&lt;class T><br>constexpr span&lt;T><br>**make_span**(T \* first, T \* last) noexcept |
| &nbsp; | &nbsp;   | template&lt;class T><br>constexpr span&lt;T><br>**make_span**(T \* ptr, index_t count) noexcept |
| &nbsp; | &nbsp;   | template&lt;class T, size_t N><br>constexpr span&lt;T,N><br>**make_span**(T (&arr)[N]) noexcept |
| &nbsp; | >= C++11 | template&lt;class T, size_t N><br>constexpr span&lt;T,N><br>**make_span**(std::array&lt;T,N> & arr) noexcept |
| &nbsp; | >= C++11 | template&lt;class T, size_t N><br>constexpr span&lt;const T,N><br>**make_span**(std::array&lt;T,N > const & arr) noexcept |
| &nbsp; | >= C++11 | template&lt;class Container><br>constexpr auto<br>**make_span**(Container & cont) -><br>&emsp;span&lt;typename Container::value_type> noexcept |
| &nbsp; | >= C++11 | template&lt;class Container><br>constexpr auto<br>**make_span**(Container const & cont) -><br>&emsp;span&lt;const typename Container::value_type> noexcept |
| &nbsp; | < C++11  | template&lt;class T><br>span&lt;T><br>**make_span**(std::vector&lt;T> & cont) noexcept |
| &nbsp; | < C++11  | template&lt;class T><br>span&lt;const T><br>**make_span**(std::vector&lt;T> const & cont) noexcept |


Configuration
-------------

### Select `std::span` or `nonstd::span`
At default, *span lite* uses `std::span` if it is available and lets you use it via namespace `nonstd`. You can however override this default and explicitly request to use `std::span` as `nonstd::span` or use span lite's `nonstd::span` via the following macros.

-D<b>nssv_CONFIG_SELECT_STD_SPAN</b>=1  
Define this to 1 to select `std::span` as `nonstd::span`. Default is undefined.

-D<b>nssv_CONFIG_SELECT_NONSTD_SPAN</b>=1  
Define this to 1 to select *span lite*'s `nonstd::span`. Default is undefined.

### Provide `make_span` functions
-D<b>span_CONFIG_PROVIDE_MAKE_SPAN</b>=1  
Define this to 1 to provide creator functions `nonstd::make_span`. Default is undefined.

### Contract violation response macros

*span-lite* provides contract violation response control as suggested in proposal [N4415](http://wg21.link/n4415).

\-D<b>span\_CONFIG\_CONTRACT\_LEVEL\_ON</b>  
Define this macro to include both `span_EXPECTS` and `span_ENSURES` in the code. This is the default case.
 
\-D<b>span\_CONFIG\_CONTRACT\_LEVEL\_OFF</b>  
Define this macro to exclude both `span_EXPECTS` and `span_ENSURES` from the code.

\-D<b>span\_CONFIG_CONTRACT\_LEVEL\_EXPECTS\_ONLY</b>  
Define this macro to include `span_EXPECTS` in the code and exclude `span_ENSURES` from the code.

\-D<b>span\_CONFIG\_CONTRACT\_LEVEL\_ENSURES\_ONLY</b>  
Define this macro to exclude `span_EXPECTS` from the code and include `span_ENSURES` in the code.

\-D<b>span\_CONFIG\_CONTRACT\_VIOLATION\_TERMINATES</b>  
Define this macro to call `std::terminate()` on a GSL contract violation in `span_EXPECTS`, `span_ENSURES`. This is the default case.

\-D<b>span\_CONFIG\_CONTRACT\_VIOLATION\_THROWS</b>  
Define this macro to throw a std::runtime_exception-derived exception `gsl::fail_fast` instead of calling `std::terminate()` on a GSL contract violation in `span_EXPECTS`, `span_ENSURES` and throw a std::exception-derived exception `narrowing_error` on discarding  information in `narrow`.


Reported to work with
---------------------
The table below mentions the compiler versions *span lite* is reported to work with.

OS        | Compiler   | Versions |
---------:|:-----------|:---------|
Windows   | Clang/LLVM | ?        |
&nbsp;    | GCC        | 7.2.0    |
&nbsp;    | Visual C++<br>(Visual Studio)| 8 (2005)?, 10 (2010), 11 (2012),<br>12 (2013), 14 (2015), 15 (2017) |
GNU/Linux | Clang/LLVM | 3.5.0, 5.0, ...    |
&nbsp;    | GCC        | 5.4.1, 6.3.0, 7.2.0 |
OS X      | ?          | ?        |


Building the tests
------------------
To build the tests you need:

- [CMake](http://cmake.org), version 3.0 or later to be installed and in your PATH.
- A [suitable compiler](#reported-to-work-with).

The [*lest* test framework](https://github.com/martinmoene/lest) is included in the [test folder](test).

The following steps assume that the [*span lite* source code](https://github.com/martinmoene/span-lite) has been cloned into a directory named `./span-lite`.

1. Create a directory for the build outputs.

        cd ./span-lite
        md build && cd build

2. Configure CMake to use the compiler of your choice (run `cmake --help` for a list).

        cmake -G "Unix Makefiles" ..

3. Build the test suite.    

        cmake --build .

4. Run the test suite.    

        ctest -V

All tests should pass, indicating your platform is supported and you are ready to use *span lite*.


Other implementations of span
------------------------------------
- Google Abseil [span](https://github.com/abseil/abseil-cpp/blob/master/absl/types/span.h).
- [Search _span c++_ on GitHub](https://github.com/search?l=C%2B%2B&q=span+c%2B%2B&type=Repositories&utf8=%E2%9C%93).


Notes and references
--------------------
*Interface and specification*
- [span on cppreference](http://en.cppreference.com/w/cpp/string/span).
- [p0122 - C++20 Proposal](http://wg21.link/p0122).
- [span in C++20 Working Draft](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4727.pdf) (to come).

*Presentations*
- TBD

*Proposals*
- [p0122 - span: bounds-safe views for sequences of objects](http://wg21.link/p0122).
- [p0091 - Template argument deduction for class templates](wg21.link/p0091)


Appendix
--------
### A.1 span lite test specification

```
span<>: Terminates construction from a nullptr and a non-zero size (C++11)
span<>: Terminates construction from two pointers in the wrong order
span<>: Terminates construction from a null pointer and a non-zero size
span<>: Terminates creation of a sub span of the first n elements for n exceeding the span
span<>: Terminates creation of a sub span of the last n elements for n exceeding the span
span<>: Terminates creation of a sub span outside the span
span<>: Terminates access outside the span
span<>: Allows to default-construct
span<>: Allows to construct from a nullptr and a zero size (C++11)
span<>: Allows to construct from two pointers
span<>: Allows to construct from two pointers to const
span<>: Allows to construct from a non-null pointer and a size
span<>: Allows to construct from a non-null pointer to const and a size
span<>: Allows to construct from a temporary pointer and a size
span<>: Allows to construct from a temporary pointer to const and a size
span<>: Allows to construct from any pointer and a zero size
span<>: Allows to construct from a C-array
span<>: Allows to construct from a const C-array
span<>: Allows to construct from a C-array with size via decay to pointer (potentially dangerous)
span<>: Allows to construct from a const C-array with size via decay to pointer (potentially dangerous)
span<>: Allows to construct from a std::array<> (C++11)
span<>: Allows to construct from a std::array<> with const data (C++11)
span<>: Allows to construct from a container (std::vector<>)
span<>: Allows to tag-construct from a container (std::vector<>)
span<>: Allows to tag-construct from a const container (std::vector<>)
span<>: Allows to copy-construct from another span of the same type
span<>: Allows to copy-construct from another span of a compatible type
span<>: Allows to copy-construct from a temporary span of the same type (C++11)
span<>: Allows to copy-assign from another span of the same type
span<>: Allows to copy-assign from a temporary span of the same type (C++11)
span<>: Allows to create a sub span of the first n elements
span<>: Allows to create a sub span of the last n elements
span<>: Allows to create a sub span starting at a given offset
span<>: Allows to create a sub span starting at a given offset with a given length
span<>: Allows forward iteration
span<>: Allows const forward iteration
span<>: Allows reverse iteration
span<>: Allows const reverse iteration
span<>: Allows to observe an element via array indexing
span<>: Allows to observe an element via call indexing
span<>: Allows to observe an element via data()
span<>: Allows to change an element via array indexing
span<>: Allows to change an element via call indexing
span<>: Allows to change an element via data()
span<>: Allows to compare equal to another span of the same type
span<>: Allows to compare unequal to another span of the same type
span<>: Allows to compare less than another span of the same type
span<>: Allows to compare less than or equal to another span of the same type
span<>: Allows to compare greater than another span of the same type
span<>: Allows to compare greater than or equal to another span of the same type
span<>: Allows to compare to another span of the same type and different cv-ness (non-standard)
span<>: Allows to compare empty spans as equal
span<>: Allows to test for empty span via empty(), empty case
span<>: Allows to test for empty span via empty(), non-empty case
span<>: Allows to obtain the number of elements via size()
span<>: Allows to obtain the number of bytes via size_bytes()
span<>: Allows to view the elements as read-only bytes
span<>: Allows to view and change the elements as writable bytes
make_span(): Allows building from two pointers
make_span(): Allows building from two const pointers
make_span(): Allows building from a non-null pointer and a size
make_span(): Allows building from a non-null const pointer and a size
make_span(): Allows building from a C-array
make_span(): Allows building from a const C-array
make_span(): Allows building from a std::array<> (C++11)
make_span(): Allows building from a const std::array<> (C++11)
make_span(): Allows building from a container (std::vector<>)
make_span(): Allows building from a const container (std::vector<>)
```
