// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/span-lite
//
// This code is licensed under the MIT License (MIT).

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

CASE( "span-lite version" "[.version]" )
{
    span_PRESENT( span_VERSION );
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
#if span_HAVE( AUTO )
    span_PRESENT( span_HAVE_AUTO );
#else
    span_ABSENT(  span_HAVE_AUTO );
#endif

#if span_HAVE( NULLPTR )
    span_PRESENT( span_HAVE_NULLPTR );
#else
    span_ABSENT(  span_HAVE_NULLPTR );
#endif

#if span_HAVE( STATIC_ASSERT )
    span_PRESENT( span_HAVE_STATIC_ASSERT );
#else
    span_ABSENT(  span_HAVE_STATIC_ASSERT );
#endif

#if span_HAVE( DEFAULT_FUNCTION_TEMPLATE_ARG )
    span_PRESENT( span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#else
    span_ABSENT(  span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#endif

#if span_HAVE( ALIAS_TEMPLATE )
    span_PRESENT( span_HAVE_ALIAS_TEMPLATE );
#else
    span_ABSENT(  span_HAVE_ALIAS_TEMPLATE );
#endif

#if span_HAVE( CONSTEXPR_11)
    span_PRESENT( span_HAVE_CONSTEXPR_11 );
#else
    span_ABSENT(  span_HAVE_CONSTEXPR_11 );
#endif

#if span_HAVE( CONSTEXPR_14 )
    span_PRESENT( span_HAVE_CONSTEXPR_14 );
#else
    span_ABSENT(  span_HAVE_CONSTEXPR_14 );
#endif

//#if span_HAVE( ENUM_CLASS )
//    span_PRESENT( span_HAVE_ENUM_CLASS );
//#else
//    span_ABSENT(  span_HAVE_ENUM_CLASS );
//#endif
//
//#if span_HAVE( ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE )
//    span_PRESENT( span_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
//#else
//    span_ABSENT(  span_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
//#endif

#if span_HAVE( EXPLICIT_CONVERSION )
    span_PRESENT( span_HAVE_EXPLICIT_CONVERSION );
#else
    span_ABSENT(  span_HAVE_EXPLICIT_CONVERSION );
#endif

//#if span_HAVE( INITIALIZER_LIST )
//    span_PRESENT( span_HAVE_INITIALIZER_LIST );
//#else
//    span_ABSENT(  span_HAVE_INITIALIZER_LIST );
//#endif

#if span_HAVE( IS_DEFAULT )
    span_PRESENT( span_HAVE_IS_DEFAULT );
#else
    span_ABSENT(  span_HAVE_IS_DEFAULT );
#endif

#if span_HAVE( IS_DELETE )
    span_PRESENT( span_HAVE_IS_DELETE );
#else
    span_ABSENT(  span_HAVE_IS_DELETE );
#endif

#if span_HAVE( NOEXCEPT )
    span_PRESENT( span_HAVE_NOEXCEPT );
#else
    span_ABSENT(  span_HAVE_NOEXCEPT );
#endif

//#if span_HAVE( REF_QUALIFIER )
//    span_PRESENT( span_HAVE_REF_QUALIFIER );
//#else
//    span_ABSENT(  span_HAVE_REF_QUALIFIER );
//#endif
}

CASE( "presence of C++ library features" "[.stdlibrary]" )
{
#if span_HAVE( ARRAY )
    span_PRESENT( span_HAVE_ARRAY );
#else
    span_ABSENT(  span_HAVE_ARRAY );
#endif

#if span_HAVE( CONDITIONAL )
    span_PRESENT( span_HAVE_CONDITIONAL );
#else
    span_ABSENT(  span_HAVE_CONDITIONAL );
#endif

#if span_HAVE( CONTAINER_DATA_METHOD )
    span_PRESENT( span_HAVE_CONTAINER_DATA_METHOD );
#else
    span_ABSENT(  span_HAVE_CONTAINER_DATA_METHOD );
#endif

#if span_HAVE( DATA )
    span_PRESENT( span_HAVE_DATA );
#else
    span_ABSENT(  span_HAVE_DATA );
#endif

#if span_HAVE( REMOVE_CONST )
    span_PRESENT( span_HAVE_REMOVE_CONST );
#else
    span_ABSENT(  span_HAVE_REMOVE_CONST );
#endif

//#if span_HAVE( REMOVE_CV )
//    span_PRESENT( span_HAVE_REMOVE_CV );
//#else
//    span_ABSENT(  span_HAVE_REMOVE_CV );
//#endif

//#if span_HAVE( SIZED_TYPES )
//    span_PRESENT( span_HAVE_SIZED_TYPES );
//#else
//    span_ABSENT(  span_HAVE_SIZED_TYPES );
//#endif

#if span_HAVE( TYPE_TRAITS )
    span_PRESENT( span_HAVE_TYPE_TRAITS );
#else
    span_ABSENT(  span_HAVE_TYPE_TRAITS );
#endif

#ifdef _HAS_CPP0X
    span_PRESENT( _HAS_CPP0X );
#else
    span_ABSENT(  _HAS_CPP0X );
#endif
}

CASE( "presence of span library features" "[.span]" )
{
#ifdef span_HAVE_WITH_CONTAINER
    span_PRESENT( span_HAVE_WITH_CONTAINER );
#else
    span_ABSENT(  span_HAVE_WITH_CONTAINER );
#endif
#ifdef span_HAVE_MAKE_SPAN
    span_PRESENT( span_HAVE_MAKE_SPAN );
#else
    span_ABSENT(  span_HAVE_MAKE_SPAN );
#endif
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
