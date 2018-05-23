// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/span-lite
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "span-main.t.hpp"

#ifndef  span_HAVE
# define span_HAVE( feature ) ( span_HAVE_##feature )
#endif

#define span_PRESENT( x ) \
    std::cout << #x << ": " << x << "\n"

#define span_ABSENT( x ) \
    std::cout << #x << ": (undefined)\n"

// Suppress:
// - unused parameter, for cases without assertions such as [.std...]
#if defined __clang__
# pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wunused-parameter"
#endif

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

CASE( "span-lite version" "[.version]" )
{
    span_PRESENT( span_lite_MAJOR );
    span_PRESENT( span_lite_MINOR );
    span_PRESENT( span_lite_PATCH );
    span_PRESENT( span_lite_VERSION );
}

CASE( "__cplusplus" "[.stdc++]" )
{
    span_PRESENT( __cplusplus );

#ifdef _MSVC_LANG
    span_PRESENT( _MSVC_LANG );
#else
    span_ABSENT(  _MSVC_LANG );
#endif

    span_PRESENT( span_CPLUSPLUS );
    span_PRESENT( span_CPLUSPLUS_V );
}

CASE( "compiler version" "[.compiler]" )
{
#ifdef span_COMPILER_CLANG_VERSION
    span_PRESENT( span_COMPILER_CLANG_VERSION );
#else
    span_ABSENT(  span_COMPILER_CLANG_VERSION );
#endif

#ifdef span_COMPILER_GNUC_VERSION
    span_PRESENT( span_COMPILER_GNUC_VERSION );
#else
    span_ABSENT(  span_COMPILER_GNUC_VERSION );
#endif

#ifdef span_COMPILER_MSVC_VERSION
    span_PRESENT( span_COMPILER_MSVC_VERSION );
#else
    span_ABSENT(  span_COMPILER_MSVC_VERSION );
#endif
}

CASE( "presence of C++ language features" "[.stdlanguage]" )
{
#if defined(__cpp_exceptions)
    span_PRESENT( __cpp_exceptions );
#else
    span_ABSENT(  __cpp_exceptions );
#endif

#if defined(__EXCEPTIONS)
    span_PRESENT( __EXCEPTIONS );
#else
    span_ABSENT(  __EXCEPTIONS );
#endif

#if defined(_CPPUNWIND)
    span_PRESENT( _CPPUNWIND );
#else
    span_ABSENT(  _CPPUNWIND );
#endif

    span_PRESENT( span_HAVE_AUTO );
    span_PRESENT( span_HAVE_NULLPTR );
    span_PRESENT( span_HAVE_STATIC_ASSERT );
    span_PRESENT( span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
    span_PRESENT( span_HAVE_ALIAS_TEMPLATE );
    span_PRESENT( span_HAVE_CONSTEXPR_11 );
    span_PRESENT( span_HAVE_CONSTEXPR_14 );
    span_PRESENT( span_HAVE_EXPLICIT_CONVERSION );
    span_PRESENT( span_HAVE_IS_DEFAULT );
    span_PRESENT( span_HAVE_IS_DELETE );
    span_PRESENT( span_HAVE_NOEXCEPT );
}

CASE( "presence of C++ library features" "[.stdlibrary]" )
{
    span_PRESENT( span_HAS_CPP0X );
    span_PRESENT( span_HAVE_ADDRESSOF );
    span_PRESENT( span_HAVE_ARRAY );
    span_PRESENT( span_HAVE_CONDITIONAL );
    span_PRESENT( span_HAVE_CONTAINER_DATA_METHOD );
    span_PRESENT( span_HAVE_DATA );
    span_PRESENT( span_HAVE_REMOVE_CONST );
    span_PRESENT( span_HAVE_SNPRINTF );
    span_PRESENT( span_HAVE_TYPE_TRAITS );
}

CASE( "span configuration" "[.span]" )
{
    span_PRESENT( span_USES_STD_SPAN );

#ifdef span_CONFIG_SELECT_STD_SPAN
    span_PRESENT( span_CONFIG_SELECT_STD_SPAN );
#else
    span_ABSENT(  span_CONFIG_SELECT_STD_SPAN );
#endif

#ifdef span_CONFIG_SELECT_NONSTD_SPAN
    span_PRESENT( span_CONFIG_SELECT_NONSTD_SPAN );
#else
    span_ABSENT(  span_CONFIG_SELECT_NONSTD_SPAN );
#endif

#ifdef span_CONFIG_NO_EXCEPTIONS
    span_PRESENT( span_CONFIG_NO_EXCEPTIONS );
#else
    span_ABSENT(  span_CONFIG_NO_EXCEPTIONS );
#endif

#ifdef span_CONFIG_CONTRACT_LEVEL_ON
    span_PRESENT( span_CONFIG_CONTRACT_LEVEL_ON );
#else
    span_ABSENT(  span_CONFIG_CONTRACT_LEVEL_ON );
#endif

#ifdef span_CONFIG_CONTRACT_LEVEL_OFF
    span_PRESENT( span_CONFIG_CONTRACT_LEVEL_OFF );
#else
    span_ABSENT(  span_CONFIG_CONTRACT_LEVEL_OFF );
#endif

#ifdef span_CONFIG_CONTRACT_LEVEL_EXPECTS_ONLY
    span_PRESENT( span_CONFIG_CONTRACT_LEVEL_EXPECTS_ONLY );
#else
    span_ABSENT(  span_CONFIG_CONTRACT_LEVEL_EXPECTS_ONLY );
#endif

#ifdef span_CONFIG_CONTRACT_LEVEL_ENSURES_ONLY
    span_PRESENT( span_CONFIG_CONTRACT_LEVEL_ENSURES_ONLY );
#else
    span_ABSENT(  span_CONFIG_CONTRACT_LEVEL_ENSURES_ONLY );
#endif

#ifdef span_CONFIG_CONTRACT_VIOLATION_THROWS
    span_PRESENT( span_CONFIG_CONTRACT_VIOLATION_THROWS );
#else
    span_ABSENT(  span_CONFIG_CONTRACT_VIOLATION_THROWS );
#endif

#ifdef span_CONFIG_CONTRACT_VIOLATION_TERMINATES
    span_PRESENT( span_CONFIG_CONTRACT_VIOLATION_TERMINATES );
#else
    span_ABSENT(  span_CONFIG_CONTRACT_VIOLATION_TERMINATES );
#endif
}

CASE( "presence of span library features" "[.span]" )
{
    span_PRESENT( span_FEATURE_BYTE_SPAN  );
    span_PRESENT( span_FEATURE_MAKE_SPAN_TO_STD );
    span_PRESENT( span_FEATURE_WITH_CONTAINER_TO_STD );
    span_PRESENT( span_FEATURE_CONSTRUCTION_FROM_STDARRAY_ELEMENT_TYPE );
    span_PRESENT( span_FEATURE_MEMBER_AT );
    span_PRESENT( span_FEATURE_MEMBER_BACK_FRONT );
    span_PRESENT( span_FEATURE_MEMBER_SWAP );
    span_PRESENT( span_FEATURE_SAME );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv );
}

#if 0
g++            -I../include/nonstd -o span-lite.t.exe span-lite.t.cpp && span-lite.t.exe --pass
g++ -std=c++98 -I../include/nonstd -o span-lite.t.exe span-lite.t.cpp && span-lite.t.exe --pass
g++ -std=c++03 -I../include/nonstd -o span-lite.t.exe span-lite.t.cpp && span-lite.t.exe --pass
g++ -std=c++0x -I../include/nonstd -o span-lite.t.exe span-lite.t.cpp && span-lite.t.exe --pass
g++ -std=c++11 -I../include/nonstd -o span-lite.t.exe span-lite.t.cpp && span-lite.t.exe --pass
g++ -std=c++14 -I../include/nonstd -o span-lite.t.exe span-lite.t.cpp && span-lite.t.exe --pass
g++ -std=c++17 -I../include/nonstd -o span-lite.t.exe span-lite.t.cpp && span-lite.t.exe --pass

cl -EHsc -I../include/nonstd span-lite.t.cpp && span-lite.t.exe --pass
#endif

// end of file
