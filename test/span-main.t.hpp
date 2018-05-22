// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/span-lite
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef TEST_SPAN_LITE_H_INCLUDED
#define TEST_SPAN_LITE_H_INCLUDED

#include "span.hpp"

#if span_HAVE( BYTE )
#include <iosfwd>
namespace lest { std::ostream & operator<<( std::ostream & os, std::byte b ); }
#endif

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

template< typename T >
inline std::ostream & operator<<( std::ostream & os, span<T> const & v )
{
    using lest::to_string;
    return os << "[span:" << (v.empty() ? "[empty]" : to_string( v(0) ) ) << "]";
}

}}

namespace lest {

using ::nonstd::span_lite::operator<<;

#if span_HAVE( BYTE )
inline std::ostream & operator<<( std::ostream & os, std::byte b )
{
    return os << "[byte:" << std::hex << std::showbase << std::to_integer<int>(b) << "]";
}
#endif

} // namespace lest

#endif // TEST_SPAN_LITE_H_INCLUDED

// end of file
