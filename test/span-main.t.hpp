// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/span-lite
//
// This code is licensed under the MIT License (MIT).

#pragma once

#ifndef TEST_OPTIONAL_LITE_H_INCLUDED
#define TEST_OPTIONAL_LITE_H_INCLUDED

#include "span.hpp"
#include "lest_cpp03.hpp"

// Compiler warning suppression:

#if defined __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wundef"
# pragma clang diagnostic ignored "-Wheader-hygiene"
# pragma clang diagnostic ignored "-Wstring-conversion"
#elif defined __GNUC__
# pragma GCC   diagnostic push
# pragma GCC   diagnostic ignored "-Wundef"
#endif

using namespace nonstd;

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace nonstd { namespace span_lite {

// use oparator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

#if span_HAVE( BYTE )
inline std::ostream & operator<<( std::ostream & os, std::byte b )
{
    return os << "[byte:" << std::hex << std::showbase << std::to_integer<int>(b) << "]";
}
#endif

template< typename T >
inline std::ostream & operator<<( std::ostream & os, span<T> const & v )
{
    using lest::to_string;
    return os << "[span:" << (v.empty() ? "[empty]" : to_string( v(0) ) ) << "]";
}

}}

namespace lest {

using ::nonstd::span_lite::operator<<;

} // namespace lest

#endif // TEST_OPTIONAL_LITE_H_INCLUDED

// end of file
