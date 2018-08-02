<a id="top"></a>
span lite - A single-file header-only version of a C++20-like span for C++98, C++11 and later
====================================================
[![Standard](https://img.shields.io/badge/c%2B%2B-98/11/14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-BSL-blue.svg)](https://opensource.org/licenses/BSL-1.0) [![Build Status](https://travis-ci.org/martinmoene/span-lite.svg?branch=master)](https://travis-ci.org/martinmoene/span-lite) [![Build status](https://ci.appveyor.com/api/projects/status/1ha3wnxtam547m8p?svg=true)](https://ci.appveyor.com/project/martinmoene/span-lite) [![Version](https://badge.fury.io/gh/martinmoene%2Fspan-lite.svg)](https://github.com/martinmoene/span-lite/releases) [![download](https://img.shields.io/badge/latest-download-blue.svg)](https://github.com/martinmoene/span-lite/blob/master/include/nonstd/span.hpp) [![Try it on wandbox](https://img.shields.io/badge/on-wandbox-blue.svg)](https://wandbox.org/permlink/2KNdsdiyZh3OE4XM) [![Try it on godbolt online](https://img.shields.io/badge/on-godbolt-blue.svg)](http://bit.ly/mm-span-godbolt)

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

std::ptrdiff_t size( nonstd::span<const int> spn )
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
*span lite* is distributed under the [Boost Software License](https://github.com/martinmoene/span-lite/blob/master/LICENSE.txt).
 

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

To construct a span from a container with compilers that cannot constrain such a single-parameter constructor to containers, *span lite* provides a constructor that takes an additional parameter of type `with_container_t`. Use `with_container` as value for this parameter. See the table below and section [configuration](#configuration).

### Construct from `std::array` with const data

*span lite* can provide construction of a span from a `std::array` with const data. See the table below and section [configuration](#configuration).

### `at()`

*span lite* can provide member function `at()` for element access. Unless exceptions have been disabled, `at()` throws std::out_of_range if the index falls outside the span. With exceptions disabled, `at(index_t)` delegates bounds checking to `operator[](index_t)`. See the table below and sections [configuration](#configuration) and [disable exceptions](#disable-exceptions).

### `back()` and `front()`

*span lite* can provide `back()` and `front()` member functions for element access. See the table below and section [configuration](#configuration).

### `swap()`

*span lite* can provide a `swap()`member function. See the table below and section [configuration](#configuration).

### `same()`

*span lite* can provide function `same()` to determine if two spans refer as identical spans to the same data via the same type. If `same()` is enabled, `operator==()` incorporates it in its comparison. See the table below and section [configuration](#configuration).

### `make_span()`

*span lite* can provide `make_span()` creator functions to compensate for the class template argument deduction that is missing from pre-C++17 compilers. See the table below and section [configuration](#configuration).

### `byte_span()`

*span lite* can provide `byte_span()` creator functions to represent an object as a span of bytes. This requires the C++17 type `std::byte` to be available. See the table below and section [configuration](#configuration).


| Kind               | std  | Function or method |                                       
|--------------------|------|--------------------|
| **Macro**          |&nbsp;| macro **`span_FEATURE_WITH_CONTAINER_TO_STD`**|
| **Types**          |&nbsp;| **with_container_t** type to disambiguate below constructors |
| **Objects**        |&nbsp;| **with_container** value to disambiguate below constructors |
| **Constructors**   |&nbsp;| macro **`span_FEATURE_CONSTRUCTION_FROM_STDARRAY_ELEMENT_TYPE`**|
| &nbsp;             |&nbsp;| template&lt;class Container><br>constexpr **span**(with_container_t, Container & cont) |
| &nbsp;             |&nbsp;| template&lt;class Container><br>constexpr **span**(with_container_t, Container const & cont) |
| &nbsp;             |&nbsp;| &nbsp; |
| **Methods**        |&nbsp;| macro **`span_FEATURE_MEMBER_AT`** |
| &nbsp;             |&nbsp;| constexpr reference **at**(index_t idx) const<br>May throw std::out_of_range exception |
| &nbsp;             |&nbsp;| &nbsp; |
| **Methods**        |&nbsp;| macro **`span_FEATURE_MEMBER_BACK_FRONT`** |
| &nbsp;             |&nbsp;| constexpr reference **back()** const noexcept  |
| &nbsp;             |&nbsp;| constexpr reference **front()** const noexcept |
| &nbsp;             |&nbsp;| &nbsp; |
| **Method**         |&nbsp;| macro **`span_FEATURE_MEMBER_SWAP`** |
| &nbsp;             |&nbsp;| constexpr void **swap**(span & other) noexcept  |
| &nbsp;             |&nbsp;| &nbsp; |
| **Free function**  |&nbsp;| macro **`span_FEATURE_SAME`** |
| &nbsp;             |&nbsp;| template&lt;class T1, index_t E1, class T2, index_t E2><br>constexpr bool<br>**same**( span<T1,E1> const & l, span<T2,E2> const & r) noexcept |
| &nbsp;             |&nbsp;| &nbsp; |
| **Free functions** |&nbsp;| macro **`span_FEATURE_MAKE_SPAN_TO_STD`** |
| &nbsp; | &nbsp;   | template&lt;class T><br>constexpr span&lt;T><br>**make_span**(T \* first, T \* last) noexcept |
| &nbsp; | &nbsp;   | template&lt;class T><br>constexpr span&lt;T><br>**make_span**(T \* ptr, index_t count) noexcept |
| &nbsp; | &nbsp;   | template&lt;class T, size_t N><br>constexpr span&lt;T,N><br>**make_span**(T (&arr)[N]) noexcept |
| &nbsp; | >= C++11 | template&lt;class T, size_t N><br>constexpr span&lt;T,N><br>**make_span**(std::array&lt;T,N> & arr) noexcept |
| &nbsp; | >= C++11 | template&lt;class T, size_t N><br>constexpr span&lt;const T,N><br>**make_span**(std::array&lt;T,N > const & arr) noexcept |
| &nbsp; | >= C++11 | template&lt;class Container><br>constexpr auto<br>**make_span**(Container & cont) -><br>&emsp;span&lt;typename Container::value_type> noexcept |
| &nbsp; | >= C++11 | template&lt;class Container><br>constexpr auto<br>**make_span**(Container const & cont) -><br>&emsp;span&lt;const typename Container::value_type> noexcept |
| &nbsp; | &nbsp;   | template&lt;class Container><br>span&lt;typename Container::value_type><br>**make_span**( with_container_t, Container & cont ) |
| &nbsp; | &nbsp;   | template&lt;class Container><br>span&lt;const typename Container::value_type><br>**make_span**( with_container_t, Container const & cont ) |
| &nbsp; | < C++11  | template&lt;class T, Allocator><br>span&lt;T><br>**make_span**(std::vector&lt;T, Allocator> & cont) |
| &nbsp; | < C++11  | template&lt;class T, Allocator><br>span&lt;const T><br>**make_span**(std::vector&lt;T, Allocator> const & cont) |
| &nbsp; | &nbsp;   | &nbsp; |                                       
| **Free functions**|&nbsp;| macro **`span_FEATURE_BYTE_SPAN`** |
| &nbsp; | &nbsp;   | template&lt;class T><br>span&lt;T, sizeof(T)><br>**byte_span**(T & t) |
| &nbsp; | &nbsp;   | template&lt;class T><br>span&lt;const T, sizeof(T)><br>**byte_span**(T const & t) |


Configuration
-------------

### Standard selection macro

\-D<b>span\_CPLUSPLUS</b>=199711L
Define this macro to override the auto-detection of the supported C++ standard, if your compiler does not set the `__cplusplus` macro correctly.

### Select `std::span` or `nonstd::span`
At default, *span lite* uses `std::span` if it is available and lets you use it via namespace `nonstd`. You can however override this default and explicitly request to use `std::span` as `nonstd::span` or use span lite's `nonstd::span` via the following macros.

-D<b>span_CONFIG_SELECT_STD_SPAN</b>=1  
Define this to 1 to select `std::span` as `nonstd::span`. Default is undefined.

-D<b>span_CONFIG_SELECT_NONSTD_SPAN</b>=1  
Define this to 1 to select *span lite*'s `nonstd::span`. Default is undefined.

### Disable exceptions
-D<b>span_CONFIG_NO_EXCEPTIONS</b>=0  
Define this to 1 if you want to compile without exceptions. If not defined, the header tries and detect if exceptions have been disabled (e.g. via `-fno-exceptions`). Disabling exceptions will force contract violation to use termination, see [contract violation macros](#contract-violation-response-macros). Default is undefined.

### Provide construction using `with_container_t`
-D<b>span_FEATURE_WITH_CONTAINER_TO_STD</b>=14  
Define this to the highest C++ language version for which to enable constructing a span using `with_container_t`, like 98, 03, 11, 14, 17, 20. Default is undefined.

### Provide construction from `std::array` with const data
-D<b>span_FEATURE_CONSTRUCTION_FROM_STDARRAY_ELEMENT_TYPE</b>=1  
Define this to 1 to enable constructing a span from a std::array with const data. Default is undefined.

### Provide `at()` member function
-D<b>span_FEATURE_MEMBER_AT</b>=1  
Define this to 1 to provide member function `at()`. Define this to 2 to include index and size in message of std::out_of_range exception. Default is undefined.

### Provide `back()` and `front()` member functions
-D<b>span_FEATURE_MEMBER_BACK_FRONT</b>=1  
Define this to 1 to provide member functions `back()` and `front()`. Default is undefined.

### Provide `swap()` member function
-D<b>span_FEATURE_MEMBER_SWAP</b>=1  
Define this to 1 to provide member function `swap()`. Default is undefined.

### Provide `same()` function
-D<b>span_FEATURE_SAME</b>=1  
Define this to 1 to provide function `same()` to test if two spans refer as identical spans to the same data via the same type. If `same()` is enabled, `operator==()` incorporates it in its comparison. Default is undefined.

### Provide `make_span()` functions
-D<b>span_FEATURE_MAKE_SPAN_TO_STD</b>=14  
Define this to the highest C++ language version for which to provide creator functions `nonstd::make_span()`. Default is undefined.

### Provide `byte_span()` functions
-D<b>span_FEATURE_BYTE_SPAN</b>=1  
Define this to 1 to provide creator functions `nonstd::byte_span()`. Default is undefined.

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
Define this macro to call `std::terminate()` on a contract violation in `span_EXPECTS`, `span_ENSURES`. This is the default case.

\-D<b>span\_CONFIG\_CONTRACT\_VIOLATION\_THROWS</b>  
Define this macro to throw an exception of implementation-defined type that is derived from `std::runtime_exception` instead of calling `std::terminate()` on a contract violation in `span_EXPECTS` and `span_ENSURES`. See also [disable exceptions](#disable-exceptions).


Reported to work with
---------------------
The table below mentions the compiler versions *span lite* is reported to work with.

OS           | Compiler   | Where   | Versions |
------------:|:-----------|:--------|:---------|
**GNU/Linux**| Clang/LLVM | Travis  | 3.5.0, 3.6.2, 3.7.1, 3.8.0, 3.9.1, 4.0.1  |
 &nbsp;      | GCC        | Travis  | 5.5.0, 6.4.0, 7.3.0 |
**OS X**     | ?          | Local   | ?        |
**Windows**  | Clang/LLVM | Local   | 6.0.0    |
&nbsp;       | GCC        | Local   | 7.2.0    |
&nbsp;       | Visual C++<br>(Visual Studio)| Local | 8 (2005), 10 (2010), 11 (2012),<br>12 (2013), 14 (2015), 15 (2017) |
&nbsp;       | Visual C++<br>(Visual Studio)| AppVeyor | 10 (2010), 11 (2012),<br>12 (2013), 14 (2015), 15 (2017) |


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

3. Optional. You can control above configuration through the following options:

    `-DSPAN_LITE_OPT_BUILD_TESTS=ON`: build the tests for span, default on  
    `-DSPAN_LITE_OPT_BUILD_EXAMPLES=OFF`: build the examples, default off  

4. Build the test suite.    

        cmake --build .

5. Run the test suite.    

        ctest -V

All tests should pass, indicating your platform is supported and you are ready to use *span lite*.


Other implementations of span
------------------------------------
- *gsl-lite* [span](https://github.com/martinmoene/gsl-lite/blob/73c4f16f2b35fc174fc2f09d44d5ab13e5c638c3/include/gsl/gsl-lite.hpp#L1221).
- Microsoft GSL [span](https://github.com/Microsoft/GSL/blob/master/include/gsl/span).
- Google Abseil [span](https://github.com/abseil/abseil-cpp/blob/master/absl/types/span.h).
- Marshall Clow's [libc++ span snippet](https://github.com/mclow/snippets/blob/master/span.cpp).
- Tristan Brindle's [Implementation of C++20's std::span for older compilers](https://github.com/tcbrindle/span).
- [Search _span c++_ on GitHub](https://github.com/search?l=C%2B%2B&q=span+c%2B%2B&type=Repositories&utf8=%E2%9C%93).


Notes and references
--------------------
*Interface and specification*
- [span on cppreference](http://en.cppreference.com/w/cpp/string/span).
- [p0122 - C++20 Proposal](http://wg21.link/p0122).
- [span in C++20 Working Draft](http://eel.is/c++draft/views).

*Presentations*
- TBD

*Proposals*
- [p0122 - span: bounds-safe views for sequences of objects](http://wg21.link/p0122).
- [p1024 - Usability Enhancements for std::span](http://wg21.link/p1024).
- [p1089 - Sizes Should Only span Unsigned](http://wg21.link/p1089).
- [p0805 - Comparing Containers](http://wg21.link/p0805)
- [p0091 - Template argument deduction for class templates](http://wg21.link/p0091)
- [p0856 - Restrict Access Property for mdspan and span](http://wg21.link/p0856)
- [lwg 3101 - span's Container constructors need another constraint](https://cplusplus.github.io/LWG/issue3101)


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
span<>: Throws  on access outside the span via at(): std::out_of_range [span_FEATURE_MEMBER_AT>0][span_CONFIG_NO_EXCEPTIONS=0]
span<>: Termination throws std::logic_error-derived exception [span_CONFIG_CONTRACT_VIOLATION_THROWS=1]
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
span<>: Allows to construct from a std::array<> with const data (C++11, span_FEATURE_CONSTR..._ELEMENT_TYPE=1)
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
span<>: Allows to observe an element via array indexing
span<>: Allows to observe an element via call indexing
span<>: Allows to observe an element via at() [span_FEATURE_MEMBER_AT>0]
span<>: Allows to observe an element via data()
span<>: Allows to observe the first element via front() [span_FEATURE_MEMBER_BACK_FRONT=1]
span<>: Allows to observe the last element via back() [span_FEATURE_MEMBER_BACK_FRONT=1]
span<>: Allows to change an element via array indexing
span<>: Allows to change an element via call indexing
span<>: Allows to change an element via at() [span_FEATURE_MEMBER_AT=1]
span<>: Allows to change an element via data()
span<>: Allows to change the first element via front() [span_FEATURE_MEMBER_BACK_FRONT=1]
span<>: Allows to change the last element via back() [span_FEATURE_MEMBER_BACK_FRONT=1]
span<>: Allows to swap with another span [span_FEATURE_MEMBER_SWAP=1]
span<>: Allows forward iteration
span<>: Allows const forward iteration
span<>: Allows reverse iteration
span<>: Allows const reverse iteration
span<>: Allows to identify if a span is the same as another span [span_FEATURE_SAME=1]
span<>: Allows to compare equal to another span of the same type
span<>: Allows to compare unequal to another span of the same type
span<>: Allows to compare less than another span of the same type
span<>: Allows to compare less than or equal to another span of the same type
span<>: Allows to compare greater than another span of the same type
span<>: Allows to compare greater than or equal to another span of the same type
span<>: Allows to compare to another span of the same type and different cv-ness [span_FEATURE_SAME=0]
span<>: Allows to compare empty spans as equal
span<>: Allows to test for empty span via empty(), empty case
span<>: Allows to test for empty span via empty(), non-empty case
span<>: Allows to obtain the number of elements via size()
span<>: Allows to obtain the number of bytes via size_bytes()
span<>: Allows to view the elements as read-only bytes
span<>: Allows to view and change the elements as writable bytes
make_span() [span_FEATURE_MAKE_SPAN_TO_STD=99]
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
make_span(): Allows building from a container (with_container_t, std::vector<>)
make_span(): Allows building from a const container (with_container_t, std::vector<>)
byte_span() [span_FEATURE_BYTE_SPAN=1]
byte_span(): Allows building a span of std::byte from a single object (C++17)
byte_span(): Allows building a span of const std::byte from a single const object (C++17)
```
