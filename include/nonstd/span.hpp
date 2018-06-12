//
// span for C++98 and later.
// Based on http://wg21.link/p0122r7
// For more information see https://github.com/martinmoene/span-lite
//
// Copyright (c) 2018 Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NONSTD_SPAN_HPP_INCLUDED
#define NONSTD_SPAN_HPP_INCLUDED

#define  span_lite_MAJOR  0
#define  span_lite_MINOR  3
#define  span_lite_PATCH  0
#define  span_lite_VERSION  span_STRINGIFY(span_lite_MAJOR) "." span_STRINGIFY(span_lite_MINOR) "." span_STRINGIFY(span_lite_PATCH)

#define span_STRINGIFY(  x )  span_STRINGIFY_( x )
#define span_STRINGIFY_( x )  #x

// Configuration:

#ifndef  span_FEATURE_WITH_CONTAINER_TO_STD
# define span_FEATURE_WITH_CONTAINER_TO_STD  0
#endif

#ifndef  span_FEATURE_CONSTRUCTION_FROM_STDARRAY_ELEMENT_TYPE 
# define span_FEATURE_CONSTRUCTION_FROM_STDARRAY_ELEMENT_TYPE  0
#endif

#ifndef  span_FEATURE_MEMBER_AT
# define span_FEATURE_MEMBER_AT  0
#endif

#ifndef  span_FEATURE_MEMBER_BACK_FRONT
# define span_FEATURE_MEMBER_BACK_FRONT  0
#endif

#ifndef  span_FEATURE_MEMBER_SWAP
# define span_FEATURE_MEMBER_SWAP  0
#endif

#ifndef  span_FEATURE_SAME
# define span_FEATURE_SAME  0
#endif

#ifndef  span_FEATURE_MAKE_SPAN_TO_STD
# define span_FEATURE_MAKE_SPAN_TO_STD  0
#endif

#ifndef  span_FEATURE_BYTE_SPAN
# define span_FEATURE_BYTE_SPAN  0
#endif

// Force use of std or nonstd span:

#ifdef   span_CONFIG_SELECT_STD_SPAN
# define span_USES_STD_SPAN  span_CONFIG_SELECT_STD_SPAN
#else
# define span_USES_STD_SPAN  0
#endif

#if    defined( span_CONFIG_SELECT_STD_SPAN    ) && span_CONFIG_SELECT_STD_SPAN && \
       defined( span_CONFIG_SELECT_NONSTD_SPAN ) && span_CONFIG_SELECT_NONSTD_SPAN
#error Please define none or one of span_CONFIG_SELECT_STD_SPAN, span_CONFIG_SELECT_NONSTD_SPAN to 1, but not both.
#endif

// Control presence of exception handling (try and auto discover):

#ifndef span_CONFIG_NO_EXCEPTIONS
# if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#  define span_CONFIG_NO_EXCEPTIONS  0
# else
#  define span_CONFIG_NO_EXCEPTIONS  1
#  undef  span_CONFIG_CONTRACT_VIOLATION_THROWS
#  undef  span_CONFIG_CONTRACT_VIOLATION_TERMINATES
#  define span_CONFIG_CONTRACT_VIOLATION_THROWS  0
#  define span_CONFIG_CONTRACT_VIOLATION_TERMINATES  1
# endif
#endif

// Control pre- and postcondition violation behaviour:

#if    defined( span_CONFIG_CONTRACT_LEVEL_ON )
# define        span_CONFIG_CONTRACT_LEVEL_MASK  0x11
#elif  defined( span_CONFIG_CONTRACT_LEVEL_OFF )
# define        span_CONFIG_CONTRACT_LEVEL_MASK  0x00
#elif  defined( span_CONFIG_CONTRACT_LEVEL_EXPECTS_ONLY )
# define        span_CONFIG_CONTRACT_LEVEL_MASK  0x01
#elif  defined( span_CONFIG_CONTRACT_LEVEL_ENSURES_ONLY )
# define        span_CONFIG_CONTRACT_LEVEL_MASK  0x10
#else
# define        span_CONFIG_CONTRACT_LEVEL_MASK  0x11
#endif

#if    defined( span_CONFIG_CONTRACT_VIOLATION_THROWS )
# define        span_CONFIG_CONTRACT_VIOLATION_THROWS_V  span_CONFIG_CONTRACT_VIOLATION_THROWS
#else
# define        span_CONFIG_CONTRACT_VIOLATION_THROWS_V  0
#endif

#if    defined( span_CONFIG_CONTRACT_VIOLATION_THROWS     ) && span_CONFIG_CONTRACT_VIOLATION_THROWS && \
       defined( span_CONFIG_CONTRACT_VIOLATION_TERMINATES ) && span_CONFIG_CONTRACT_VIOLATION_TERMINATES
# error Please define none or one of span_CONFIG_CONTRACT_VIOLATION_THROWS and span_CONFIG_CONTRACT_VIOLATION_TERMINATES to 1, but not both.
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   span_CPLUSPLUS
# ifdef  _MSVC_LANG
#  define span_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define span_CPLUSPLUS  __cplusplus
# endif
#endif

#define span_CPP98_OR_GREATER  ( span_CPLUSPLUS >= 199711L )
#define span_CPP11_OR_GREATER  ( span_CPLUSPLUS >= 201103L )
#define span_CPP14_OR_GREATER  ( span_CPLUSPLUS >= 201402L )
#define span_CPP17_OR_GREATER  ( span_CPLUSPLUS >= 201703L )
#define span_CPP20_OR_GREATER  ( span_CPLUSPLUS >= 202000L )

// C++ language version (represent 98 as 3):

#define span_CPLUSPLUS_V  ( span_CPLUSPLUS / 100 - (span_CPLUSPLUS > 200000 ? 2000 : 1994) )

#define span_IN_STD( v )  ( (v) == 98 || (v) >= span_CPLUSPLUS_V )

#define span_CONFIG(         feature )  ( span_CONFIG_##feature )
#define span_FEATURE(        feature )  ( span_FEATURE_##feature )
#define span_FEATURE_TO_STD( feature )  ( span_IN_STD( span_FEATURE( feature##_TO_STD ) ) )

// use C++20 std::span if available:

#ifdef __has_include
# define span_HAS_INCLUDE( arg )  __has_include( arg )
#else
# define span_HAS_INCLUDE( arg )  0
#endif

#define  span_HAVE_STD_SPAN  ( span_CPP20_OR_GREATER && span_HAS_INCLUDE( <span> ) )

#ifndef  span_USES_STD_SPAN
# define span_USES_STD_SPAN  span_HAVE_STD_SPAN
#endif

//
// Use C++20 std::span:
//

#if span_USES_STD_SPAN

#include <span>

namespace nonstd {

using std::span;

using std::operator==;
using std::operator!=;
using std::operator<;
using std::operator<=;
using std::operator>;
using std::operator>=;
}  // namespace nonstd

#else  // span_USES_STD_SPAN

#include <algorithm>

// Compiler versions:
//
// MSVC++ 6.0  _MSC_VER == 1200 (Visual Studio 6.0)
// MSVC++ 7.0  _MSC_VER == 1300 (Visual Studio .NET 2002)
// MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio .NET 2003)
// MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
// MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 14.1 _MSC_VER >= 1910 (Visual Studio 2017)

#if defined( _MSC_VER ) && !defined( __clang__ )
# define span_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define span_COMPILER_MSVC_VERSION  0
#endif

#define span_COMPILER_VERSION( major, minor, patch )  ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined __clang__
# define span_COMPILER_CLANG_VERSION  span_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define span_COMPILER_CLANG_VERSION  0
#endif

#if defined __GNUC__
# define span_COMPILER_GNUC_VERSION  span_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define span_COMPILER_GNUC_VERSION  0
#endif

// half-open range [lo..hi):
#define span_BETWEEN( v, lo, hi )  ( (lo) <= (v) && (v) < (hi) )

// Compiler warning suppression:

#if defined __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wundef"
# define span_RESTORE_WARNINGS()   _Pragma( "clang diagnostic pop" )

#elif defined __GNUC__
# pragma GCC   diagnostic push
# pragma GCC   diagnostic ignored "-Wundef"
# define span_RESTORE_WARNINGS()   _Pragma( "GCC diagnostic pop" )

#elif span_COMPILER_MSVC_VERSION >= 140
# define span_DISABLE_MSVC_WARNINGS(codes)  __pragma(warning(push))  __pragma(warning(disable: codes))
# define span_RESTORE_WARNINGS()            __pragma(warning(pop ))

// Suppress the following MSVC GSL warnings:
// - C26439, gsl::f.6 : special function 'function' can be declared 'noexcept'
// - C26440, gsl::f.6 : function 'function' can be declared 'noexcept'
// - C26472, gsl::t.1 : don't use a static_cast for arithmetic conversions;
//                      use brace initialization, gsl::narrow_cast or gsl::narrow
// - C26473: gsl::t.1 : don't cast between pointer types where the source type and the target type are the same
// - C26481: gsl::b.1 : don't use pointer arithmetic. Use span instead
// - C26490: gsl::t.1 : don't use reinterpret_cast

span_DISABLE_MSVC_WARNINGS( 26439 26440 26472 26473 26481 26490 )

#else
# define span_RESTORE_WARNINGS()  /*empty*/
#endif

// Presence of language and library features:

#define span_HAVE( feature )  ( span_HAVE_##feature )

#ifdef _HAS_CPP0X
# define span_HAS_CPP0X  _HAS_CPP0X
#else
# define span_HAS_CPP0X  0
#endif

#define span_CPP11_80   (span_CPP11_OR_GREATER || span_COMPILER_MSVC_VERSION >= 80)
#define span_CPP11_90   (span_CPP11_OR_GREATER || span_COMPILER_MSVC_VERSION >= 90)
#define span_CPP11_100  (span_CPP11_OR_GREATER || span_COMPILER_MSVC_VERSION >= 100)
#define span_CPP11_110  (span_CPP11_OR_GREATER || span_COMPILER_MSVC_VERSION >= 110)
#define span_CPP11_120  (span_CPP11_OR_GREATER || span_COMPILER_MSVC_VERSION >= 120)
#define span_CPP11_140  (span_CPP11_OR_GREATER || span_COMPILER_MSVC_VERSION >= 140)

#define span_CPP14_000  (span_CPP14_OR_GREATER)
#define span_CPP14_120  (span_CPP14_OR_GREATER || span_COMPILER_MSVC_VERSION >= 120)
#define span_CPP14_140  (span_CPP14_OR_GREATER || span_COMPILER_MSVC_VERSION >= 140)

#define span_CPP17_000  (span_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define span_HAVE_AUTO                  span_CPP11_100
#define span_HAVE_NULLPTR               span_CPP11_100
#define span_HAVE_STATIC_ASSERT         span_CPP11_100

#define span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG \
                                        span_CPP11_120

#define span_HAVE_ALIAS_TEMPLATE        span_CPP11_140
#define span_HAVE_CONSTEXPR_11          span_CPP11_140
#define span_HAVE_EXPLICIT_CONVERSION   span_CPP11_140
#define span_HAVE_IS_DEFAULT            span_CPP11_140
#define span_HAVE_IS_DELETE             span_CPP11_140
#define span_HAVE_NOEXCEPT              span_CPP11_140

// Presence of C++14 language features:

#define span_HAVE_CONSTEXPR_14          span_CPP14_000

// Presence of C++17 language features:

#define span_HAVE_NODISCARD             span_CPP17_000
#define span_HAVE_NORETURN              span_CPP17_000

// MSVC: template parameter deduction guides since Visual Studio 2017 v15.7

#define span_HAVE_DEDUCTION_GUIDES     (span_CPP17_OR_GREATER && ! span_BETWEEN( span_COMPILER_MSVC_VERSION, 1, 999 ))

// Presence of C++ library features:

#define span_HAVE_TYPE_TRAITS           span_CPP11_90

#define span_HAVE_ARRAY                 span_CPP11_110
#define span_HAVE_LONGLONG              span_CPP11_80
#define span_HAVE_REMOVE_CONST          span_CPP11_110
#define span_HAVE_SNPRINTF              span_CPP11_140

#define span_HAVE_CONDITIONAL           span_CPP11_120

#define span_HAVE_CONTAINER_DATA_METHOD (span_CPP11_140 || ( span_COMPILER_MSVC_VERSION >= 90 && span_HAS_CPP0X ))

#define span_HAVE_ADDRESSOF             span_CPP17_000
#define span_HAVE_BYTE                  span_CPP17_000
#define span_HAVE_DATA                  span_CPP17_000

// C++ feature usage:

#if span_HAVE_ADDRESSOF
# define span_ADDRESSOF(x)  std::addressof(x)
#else
# define span_ADDRESSOF(x)  (&x)
#endif

#if span_HAVE_CONSTEXPR_11
# define span_constexpr constexpr
#else
# define span_constexpr /*span_constexpr*/
#endif

#if span_HAVE_CONSTEXPR_14
# define span_constexpr14 constexpr
#else
# define span_constexpr14 /*span_constexpr*/
#endif

#if span_HAVE_EXPLICIT_CONVERSION
# define span_explicit explicit
#else
# define span_explicit /*explicit*/
#endif

#if span_HAVE_IS_DELETE
# define span_is_delete = delete
#else
# define span_is_delete
#endif

#if span_HAVE_IS_DELETE
# define span_is_delete_access public
#else
# define span_is_delete_access private
#endif

#if span_HAVE_NOEXCEPT && ! span_CONFIG_CONTRACT_VIOLATION_THROWS_V
# define span_noexcept noexcept
#else
# define span_noexcept /*noexcept*/
#endif

#if span_HAVE_NULLPTR
# define span_nullptr nullptr
#else
# define span_nullptr NULL
#endif

#if span_HAVE_NODISCARD
# define span_nodiscard [[nodiscard]]
#else
# define span_nodiscard /*[[nodiscard]]*/
#endif

#if span_HAVE_NORETURN
# define span_noreturn [[noreturn]]
#else
# define span_noreturn /*[[noreturn]]*/
#endif

// Other features:

#define span_HAVE_CONSTRAINED_SPAN_CONTAINER_CTOR  \
    ( span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG && span_HAVE_CONTAINER_DATA_METHOD )

// Additional includes:

#if span_HAVE( ADDRESSOF )
# include <memory>
#endif

#if span_HAVE( ARRAY )
# include <array>
#endif

#if span_HAVE( BYTE )
# include <cstddef>
#endif

#if span_HAVE( TYPE_TRAITS )
# include <type_traits>
#endif

#if ! span_HAVE( CONSTRAINED_SPAN_CONTAINER_CTOR )
# include <vector>
#endif

#if span_FEATURE( MEMBER_AT ) > 1
# include <cstdio>
#endif

#if span_CONFIG( CONTRACT_VIOLATION_THROWS_V )
# include <stdexcept>
#endif

// Contract violation

#define span_ELIDE_CONTRACT_EXPECTS  ( 0 == ( span_CONFIG_CONTRACT_LEVEL_MASK & 0x01 ) )
#define span_ELIDE_CONTRACT_ENSURES  ( 0 == ( span_CONFIG_CONTRACT_LEVEL_MASK & 0x10 ) )

#if span_ELIDE_CONTRACT_EXPECTS
# define span_constexpr_exp    span_constexpr
# define span_EXPECTS( cond )  /* Expect elided */
#else
# define span_constexpr_exp    span_constexpr14
# define span_EXPECTS( cond )  span_CONTRACT_CHECK( "Precondition", cond )
#endif

#if span_ELIDE_CONTRACT_ENSURES
# define span_constexpr_ens    span_constexpr
# define span_ENSURES( cond )  /* Ensures elided */
#else
# define span_constexpr_ens    span_constexpr14
# define span_ENSURES( cond )  span_CONTRACT_CHECK( "Postcondition", cond )
#endif

#define span_CONTRACT_CHECK( type, cond ) \
    cond ? static_cast< void >( 0 ) \
         : nonstd::span_lite::detail::report_contract_violation( span_LOCATION( __FILE__, __LINE__ ) ": " type " violation." )

#ifdef __GNUG__
# define span_LOCATION( file, line )  file ":" span_STRINGIFY( line )
#else
# define span_LOCATION( file, line )  file "(" span_STRINGIFY( line ) ")"
#endif

namespace nonstd {
namespace span_lite {

// [views.constants], constants

typedef std::ptrdiff_t index_t;

span_constexpr const index_t dynamic_extent = -1;

template< class T, index_t Extent = dynamic_extent >
class span;

// Tag to select span constructor taking a container (prevent ms-gsl warning C26426):

struct with_container_t { span_constexpr with_container_t() span_noexcept {} };
const  span_constexpr   with_container_t with_container;

// Implementation details:

namespace detail {

#if span_HAVE( TYPE_TRAITS )
using std::is_same;
using std::true_type;
using std::false_type;
#endif

#if span_HAVE( REMOVE_CONST )

using std::remove_cv;
using std::remove_const;
using std::remove_volatile;

#else

template< class T > struct remove_const            { typedef T type; };
template< class T > struct remove_const< T const > { typedef T type; };

template< class T > struct remove_volatile               { typedef T type; };
template< class T > struct remove_volatile< T volatile > { typedef T type; };

template< class T >
struct remove_cv
{
    typedef typename detail::remove_volatile< typename detail::remove_const< T >::type >::type type;
};

#endif  // span_HAVE( REMOVE_CONST )

#if ! span_HAVE( TYPE_TRAITS )

struct true_type { enum { value = true }; };
struct false_type{ enum { value = false }; };

template< class T, class U > struct is_same : false_type{};
template< class T          > struct is_same<T, T> : true_type{};

#endif

#if span_HAVE( TYPE_TRAITS )

template< class Q >
struct is_span_oracle : std::false_type{};

template< class T, std::ptrdiff_t Extent >
struct is_span_oracle< span<T, Extent> > : std::true_type{};

template< class Q >
struct is_span : is_span_oracle< typename std::remove_cv<Q>::type >{};

template< class Q >
struct is_std_array_oracle : std::false_type{};

#if span_HAVE( ARRAY )

template< class T, std::size_t Extent >
struct is_std_array_oracle< std::array<T, Extent> > : std::true_type{};

#endif

template< class Q >
struct is_std_array : is_std_array_oracle< typename std::remove_cv<Q>::type >{};

template< class Q >
struct is_array : std::false_type {};

template< class T >
struct is_array<T[]> : std::true_type {};

template< class T, std::size_t N >
struct is_array<T[N]> : std::true_type {};

#endif // span_HAVE( TYPE_TRAITS )

#if ! span_CONFIG( NO_EXCEPTIONS )
#if   span_FEATURE( MEMBER_AT ) > 1

// format index and size:

#if defined __clang__
# pragma clang diagnostic ignored "-Wlong-long"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wformat=ll"
# pragma GCC   diagnostic ignored "-Wlong-long"
#endif

inline void throw_out_of_range( index_t idx, index_t size )
{
    const char fmt[] = "span::at(): index '%lli' is out of range [0..%lli)";
    char buffer[ 2 * 20 + sizeof fmt ];
    sprintf( buffer, fmt, static_cast<long long>(idx), static_cast<long long>(size) );
    
    throw std::out_of_range( buffer );
}

#else // MEMBER_AT

inline void throw_out_of_range( index_t /*idx*/, index_t /*size*/ )
{
    throw std::out_of_range( "span::at(): index outside span" );
}
#endif  // MEMBER_AT
#endif  // NO_EXCEPTIONS

#if span_CONFIG( CONTRACT_VIOLATION_THROWS_V )

struct contract_violation : std::logic_error
{
    explicit contract_violation( char const * const message )
        : std::logic_error( message )
    {}
};

inline void report_contract_violation( char const * msg )
{
    throw contract_violation( msg );
}

#else // span_CONFIG( CONTRACT_VIOLATION_THROWS_V )

span_noreturn inline void report_contract_violation( char const * /*msg*/ ) span_noexcept
{
    std::terminate();
}

#endif // span_CONFIG( CONTRACT_VIOLATION_THROWS_V )

}  // namespace detail

// Prevent signed-unsigned mismatch:

template< class T >
inline span_constexpr index_t to_size( T size )
{
    return static_cast<index_t>( size );
}

#if span_HAVE( CONSTRAINED_SPAN_CONTAINER_CTOR )

// Can construct from containers that:

template< 
    class Container, class ElementType 
    , class = typename std::enable_if<
        ! detail::is_span< Container >::value &&
        ! detail::is_array< Container >::value &&
        ! detail::is_std_array< Container >::value &&
          std::is_convertible<typename std::remove_pointer<decltype(std::declval<Container>().data())>::type(*)[], ElementType(*)[] >::value
    >::type
#if span_HAVE( DATA )
      // data(cont) and size(cont) well-formed:
    , class = decltype( std::data( std::declval<Container>() ) )
    , class = decltype( std::size( std::declval<Container>() ) )
#endif
>
struct can_construct_from : detail::true_type{};

#endif

//
// [views.span] - A view over a contiguous, single-dimension sequence of objects
//
template< class T, index_t Extent /*= dynamic_extent*/ >
class span
{
public:
    // constants and types

    typedef T element_type;
    typedef typename detail::remove_cv< T >::type value_type;

    typedef T &       reference;
    typedef T *       pointer;
    typedef T const * const_pointer;
    typedef T const & const_reference;

    typedef index_t   index_type;
    typedef index_t   difference_type;

    typedef pointer       iterator;
    typedef const_pointer const_iterator;

    typedef std::reverse_iterator< iterator >       reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;

//    static constexpr index_type extent = Extent;
    enum { extent = Extent };

    // 26.7.3.2 Constructors, copy, and assignment [span.cons]

#if span_HAVE( DEFAULT_FUNCTION_TEMPLATE_ARG )
    template< bool Dependent = false
        , class = typename std::enable_if< (Dependent || Extent <= 0) >::type >
#endif
    span_constexpr span() span_noexcept
        : data_( span_nullptr )
        , size_( 0 )
    {
        // span_EXPECTS( data() == span_nullptr );
        // span_EXPECTS( size() == 0 );
    }

    span_constexpr_exp span( pointer ptr, index_type count )
        : data_( ptr )
        , size_( count )
    {
        span_EXPECTS(
            ( ptr == span_nullptr && count == 0 ) ||
            ( ptr != span_nullptr && count >= 0 )
        );
    }

    span_constexpr_exp span( pointer firstElem, pointer lastElem )
        : data_( firstElem )
        , size_( std::distance( firstElem, lastElem ) )
    {
        span_EXPECTS(
            std::distance( firstElem, lastElem ) >= 0
        );
    }

    template< size_t N
#if span_HAVE( DEFAULT_FUNCTION_TEMPLATE_ARG )
        , class = typename std::enable_if<
            (Extent == dynamic_extent || Extent == N) &&
            std::is_convertible<value_type(*)[], element_type(*)[] >::value
        >::type
#endif
    >
    span_constexpr span( element_type ( &arr )[ N ] ) span_noexcept
        : data_( span_ADDRESSOF( arr[0] ) )
        , size_( N  )
    {}

#if span_HAVE( ARRAY )

    template< size_t N
# if span_HAVE( DEFAULT_FUNCTION_TEMPLATE_ARG )
        , class = typename std::enable_if<
            (Extent == dynamic_extent || Extent == N) &&
            std::is_convertible<value_type(*)[], element_type(*)[] >::value
        >::type
# endif
    >
# if span_FEATURE( CONSTRUCTION_FROM_STDARRAY_ELEMENT_TYPE )
        span_constexpr span( std::array< element_type, N > & arr ) span_noexcept
# else
        span_constexpr span( std::array< value_type, N > & arr ) span_noexcept
# endif
        : data_( span_ADDRESSOF( arr[0] ) )
        , size_( to_size( arr.size() ) )
    {}

    template< size_t N
# if span_HAVE( DEFAULT_FUNCTION_TEMPLATE_ARG )
        , class = typename std::enable_if<
            (Extent == dynamic_extent || Extent == N) &&
            std::is_convertible<value_type(*)[], element_type(*)[] >::value
        >::type
# endif
    >
    span_constexpr span( std::array< value_type, N> const & arr ) span_noexcept
        : data_( span_ADDRESSOF( arr[0] ) )
        , size_( to_size( arr.size() ) )
    {}

#endif // span_HAVE( ARRAY )

#if span_HAVE( CONSTRAINED_SPAN_CONTAINER_CTOR )
    template< class Container
        , class = typename std::enable_if<
            can_construct_from< Container, element_type >::value
        >::type
    >
    span_constexpr span( Container & cont )
        : data_( cont.data() )
        , size_( to_size( cont.size() ) )
    {}

    template< class Container
        , class = typename std::enable_if<
            std::is_const< element_type >::value &&
            can_construct_from< Container, element_type >::value
        >::type
    >
    span_constexpr span( Container const & cont )
        : data_( cont.data() )
        , size_( to_size( cont.size() ) )
    {}

#endif // span_HAVE( CONSTRAINED_SPAN_CONTAINER_CTOR )

#if span_FEATURE_TO_STD( WITH_CONTAINER )

    template< class Container >
    span_constexpr span( with_container_t, Container & cont )
        : data_( cont.size() == 0 ? span_nullptr : span_ADDRESSOF( cont[0] ) )
        , size_( to_size( cont.size() ) )
    {}

    template< class Container >
    span_constexpr span( with_container_t, Container const & cont )
        : data_( cont.size() == 0 ? span_nullptr : const_cast<pointer>( span_ADDRESSOF( cont[0] ) ) )
        , size_( to_size( cont.size() ) )
    {}
#endif

#if span_HAVE( IS_DEFAULT )
    span_constexpr span( span const & other ) span_noexcept = default;

    ~span() span_noexcept = default;

    span_constexpr14 span & operator=( span const & other ) span_noexcept = default;
#else
    span_constexpr span( span const & other ) span_noexcept
        : data_( other.data_ )
        , size_( other.size_ )
    {}

    ~span() span_noexcept
    {}

    span_constexpr14 span & operator=( span const & other ) span_noexcept
    {
        data_ = other.data_;
        size_ = other.size_;

        return *this;
    }
#endif

    template< class OtherElementType, index_type OtherExtent
#if span_HAVE( DEFAULT_FUNCTION_TEMPLATE_ARG )
        , class = typename std::enable_if<
            (Extent == dynamic_extent || Extent == OtherExtent) &&
            std::is_convertible<OtherElementType(*)[], element_type(*)[]>::value
        >::type
#endif
    >
    span_constexpr_exp span( span<OtherElementType, OtherExtent> const & other ) span_noexcept
        : data_( reinterpret_cast<pointer>( other.data() ) )
        , size_( other.size() )
    {
        span_EXPECTS( OtherExtent == dynamic_extent || other.size() == OtherExtent );
    }

    // 26.7.3.3 Subviews [span.sub]

    template< index_type Count >
    span_constexpr_exp span< element_type, Count >
    first() const
    {
        span_EXPECTS( 0 <= Count && Count <= size() );

        return span< element_type, Count >( data(), Count );
    }

    template< index_type Count >
    span_constexpr_exp span< element_type, Count >
    last() const
    {
        span_EXPECTS( 0 <= Count && Count <= size() );

        return span< element_type, Count >( data() + (size() - Count), Count );
    }

#if span_HAVE( DEFAULT_FUNCTION_TEMPLATE_ARG )
    template< index_type Offset, index_type Count = dynamic_extent >
#else
    template< index_type Offset, index_type Count /*= dynamic_extent*/ >
#endif
    span_constexpr_exp span< element_type, Count >
    subspan() const
    {
        span_EXPECTS(
            ( 0 <= Offset && Offset < size() ) &&
            ( Count == dynamic_extent || (0 <= Count && Count + Offset <= size()) )
        );

        return span< element_type, Count >( data() + Offset, Count != dynamic_extent ? Count : (Extent != dynamic_extent ? Extent - Offset : size() - Offset) );
    }

    span_constexpr_exp span< element_type, dynamic_extent >
    first( index_type count ) const
    {
        span_EXPECTS( 0 <= count && count <= size() );

        return span< element_type, dynamic_extent >( data(), count );
    }

    span_constexpr_exp span< element_type, dynamic_extent >
    last( index_type count ) const
    {
        span_EXPECTS( 0 <= count && count <= size() );

        return span< element_type, dynamic_extent >( data() + ( size() - count ), count );
    }

    span_constexpr_exp span< element_type, dynamic_extent >
    subspan( index_type offset, index_type count = dynamic_extent ) const
    {
        span_EXPECTS(
            ( ( 0 <= offset  && offset < size() ) ) &&
            ( count  == dynamic_extent || ( 0 <= count && offset + count <= size() ) )
        );

        return span< element_type, dynamic_extent >( data() + offset, count == dynamic_extent ? size() - offset : count );
    }

    // 26.7.3.4 Observers [span.obs]

    span_constexpr index_type size() const span_noexcept
    {
        return size_;
    }

    span_constexpr index_type size_bytes() const span_noexcept
    {
        return size() * to_size( sizeof( element_type ) );
    }

    span_nodiscard span_constexpr bool empty() const span_noexcept
    {
        return size() == 0;
    }

    // 26.7.3.5 Element access [span.elem]

    span_constexpr_exp reference operator[]( index_type idx ) const
    {
        span_EXPECTS( 0 <= idx && idx < size() );

        return *( data() + idx );
    }

    span_constexpr_exp reference operator()( index_type idx ) const
    {
        span_EXPECTS( 0 <= idx && idx < size() );

        return *( data() + idx );
    }

#if span_FEATURE( MEMBER_AT )
    span_constexpr14 reference at( index_type idx ) const
    {
#if span_CONFIG( NO_EXCEPTIONS )
        return this->operator[]( idx );
#else
        if ( idx < 0 || size() <= idx ) 
        {
            detail::throw_out_of_range( idx, size() );
        }
        return *( data() + idx );
#endif
    }
#endif

    span_constexpr pointer data() const span_noexcept
    {
        return data_;
    }

#if span_FEATURE( MEMBER_BACK_FRONT )

    span_constexpr_exp reference front() const span_noexcept
    {
        span_EXPECTS( ! empty() );

        return *data();
    }

    span_constexpr_exp reference back() const span_noexcept
    {
        span_EXPECTS( ! empty() );

        return *( data() + size() - 1 );
    }

#endif

    // xx.x.x.x Modifiers [span.modifiers]

#if span_FEATURE( MEMBER_SWAP )

    span_constexpr14 void swap( span & other ) span_noexcept
    {
        using std::swap;
        swap( data_, other.data_ );
        swap( size_, other.size_ );
    }
#endif

    // 26.7.3.6 Iterator support [span.iterators]

    span_constexpr iterator begin() const span_noexcept
    {
#if span_CPP11_OR_GREATER
        return { data() };
#else
        return iterator( data() );
#endif
    }

    span_constexpr iterator end() const span_noexcept
    {
#if span_CPP11_OR_GREATER
        return { data() + size() };
#else
        return iterator( data() + size() );
#endif
    }

    span_constexpr const_iterator cbegin() const span_noexcept
    {
#if span_CPP11_OR_GREATER
        return { data() };
#else
        return const_iterator( data() );
#endif
    }

    span_constexpr const_iterator cend() const span_noexcept
    {
#if span_CPP11_OR_GREATER
        return { data() + size() };
#else
        return const_iterator( data() + size() );
#endif
    }

    span_constexpr reverse_iterator rbegin() const span_noexcept
    {
        return reverse_iterator( end() );
    }

    span_constexpr reverse_iterator rend() const span_noexcept
    {
        return reverse_iterator( begin() );
    }

    span_constexpr const_reverse_iterator crbegin() const span_noexcept
    {
        return const_reverse_iterator ( cend() );
    }

    span_constexpr const_reverse_iterator crend() const span_noexcept
    {
        return const_reverse_iterator( cbegin() );
    }

private:
    pointer    data_;
    index_type size_;
};

// class template argument deduction guides:

#if span_HAVE( DEDUCTION_GUIDES )   // span_CPP17_OR_GREATER

template< class T, size_t N >
span( T (&)[N] ) -> span<T, N>;

template< class T, size_t N >
span( std::array<T, N> & ) -> span<T, N>;

template< class T, size_t N >
span( std::array<T, N> const & ) -> span<const T, N>;

template< class Container >
span( Container& ) -> span<typename Container::value_type>;

template< class Container >
span( Container const & ) -> span<const typename Container::value_type>;

#endif // span_HAVE( DEDUCTION_GUIDES )

// 26.7.3.7 Comparison operators [span.comparison]

#if span_FEATURE( SAME )

template< class T1, index_t E1, class T2, index_t E2  >
inline span_constexpr bool same( span<T1,E1> const & l, span<T2,E2> const & r ) span_noexcept
{
    return detail::is_same<T1, T2>::value
        && l.size() == r.size()
        && static_cast<void const*>( l.data() ) == r.data();
}

#endif

template< class T1, index_t E1, class T2, index_t E2  >
inline span_constexpr bool operator==( span<T1,E1> const & l, span<T2,E2> const & r )
{
    return
#if span_FEATURE( SAME )
        same( l, r ) ||
#endif
        ( l.size() == r.size() && std::equal( l.begin(), l.end(), r.begin() ) );
}

template< class T1, index_t E1, class T2, index_t E2  >
inline span_constexpr bool operator<( span<T1,E1> const & l, span<T2,E2> const & r )
{
    return std::lexicographical_compare( l.begin(), l.end(), r.begin(), r.end() );
}

template< class T1, index_t E1, class T2, index_t E2  >
inline span_constexpr bool operator!=( span<T1,E1> const & l, span<T2,E2> const & r )
{
    return !( l == r );
}

template< class T1, index_t E1, class T2, index_t E2  >
inline span_constexpr bool operator<=( span<T1,E1> const & l, span<T2,E2> const & r )
{
    return !( r < l );
}

template< class T1, index_t E1, class T2, index_t E2  >
inline span_constexpr bool operator>( span<T1,E1> const & l, span<T2,E2> const & r )
{
    return ( r < l );
}

template< class T1, index_t E1, class T2, index_t E2  >
inline span_constexpr bool operator>=( span<T1,E1> const & l, span<T2,E2> const & r )
{
    return !( l < r );
}

// 26.7.2.6 views of object representation [span.objectrep]

#if span_HAVE( BYTE )

template< class T, index_t Extent >
inline span_constexpr span< const std::byte, ( (Extent == dynamic_extent) ? dynamic_extent : (to_size(sizeof(T)) * Extent) ) >
as_bytes( span<T,Extent> spn ) span_noexcept
{
#if 0
    return { reinterpret_cast< std::byte const * >( spn.data() ), spn.size_bytes() };
#else
    return span< const std::byte, ( (Extent == dynamic_extent) ? dynamic_extent : (to_size(sizeof(T)) * Extent) ) >(
        reinterpret_cast< std::byte const * >( spn.data() ), spn.size_bytes() );  // NOLINT
#endif
}

template< class T, index_t Extent >
inline span_constexpr span< std::byte, ( (Extent == dynamic_extent) ? dynamic_extent : (to_size(sizeof(T)) * Extent) ) >
as_writeable_bytes( span<T,Extent> spn ) span_noexcept
{
#if 0
    return { reinterpret_cast< std::byte * >( spn.data() ), spn.size_bytes() };
#else
    return span< std::byte, ( (Extent == dynamic_extent) ? dynamic_extent : (to_size(sizeof(T)) * Extent) ) >(
        reinterpret_cast< std::byte * >( spn.data() ), spn.size_bytes() );  // NOLINT
#endif
}

#endif  // span_HAVE( BYTE )

}  // namespace span_lite
}  // namespace nonstd

// make available in nonstd:

namespace nonstd {

using span_lite::dynamic_extent;

using span_lite::span;

using span_lite::with_container;

using span_lite::operator==;
using span_lite::operator!=;
using span_lite::operator<;
using span_lite::operator<=;
using span_lite::operator>;
using span_lite::operator>=;

#if span_HAVE( BYTE )
using span_lite::as_bytes;
using span_lite::as_writeable_bytes;
#endif

#if span_FEATURE( SAME )
using span_lite::same;
#endif
}  // namespace nonstd

#endif  // span_USES_STD_SPAN

// make_span() [span-lite extension]:

#if span_FEATURE_TO_STD( MAKE_SPAN )

namespace nonstd {
namespace span_lite {

template< class T >
inline span_constexpr span<T>
make_span( T * ptr, index_t count ) span_noexcept
{
    return span<T>( ptr, count );
}

template< class T >
inline span_constexpr span<T>
make_span( T * first, T * last ) span_noexcept
{
    return span<T>( first, last );
}

template< class T, size_t N >
inline span_constexpr span<T,N>
make_span( T ( &arr )[ N ] ) span_noexcept
{
    return span<T,N>( &arr[ 0 ], N );
}

#if span_USES_STD_SPAN || span_HAVE( ARRAY )

template< class T, size_t N >
inline span_constexpr span<T,N>
make_span( std::array< T, N > & arr ) span_noexcept
{
    return span<T,N>( arr );
}

template< class T, size_t N >
inline span_constexpr span< const T, N >
make_span( std::array< T, N > const & arr ) span_noexcept
{
    return span<const T,N>( arr );
}

#endif // span_HAVE( ARRAY )

#if span_USES_STD_SPAN || ( span_HAVE( CONSTRAINED_SPAN_CONTAINER_CTOR ) && span_HAVE( AUTO ) )

template< class Container, class = decltype(std::declval<Container>().data()) >
inline span_constexpr auto
make_span( Container & cont ) span_noexcept -> span< typename Container::value_type >
{
    return span< typename Container::value_type >( cont );
}

template< class Container, class = decltype(std::declval<Container>().data()) >
inline span_constexpr auto
make_span( Container const & cont ) span_noexcept -> span< const typename Container::value_type >
{
    return span< const typename Container::value_type >( cont );
}

#else

template< class T, class Allocator >
inline span_constexpr span<T>
make_span( std::vector<T, Allocator> & cont ) span_noexcept
{
    return span<T>( with_container, cont );
}

template< class T, class Allocator >
inline span_constexpr span<const T>
make_span( std::vector<T, Allocator> const & cont ) span_noexcept
{
    return span<const T>( with_container, cont );
}

#endif // span_USES_STD_SPAN || ( ... )

#if ! span_USES_STD_SPAN && span_FEATURE_TO_STD( WITH_CONTAINER )

template< class Container >
inline span_constexpr span<typename Container::value_type>
make_span( with_container_t, Container & cont ) span_noexcept
{
    return span< typename Container::value_type >( with_container, cont );
}

template< class Container >
inline span_constexpr span<const typename Container::value_type>
make_span( with_container_t, Container const & cont ) span_noexcept
{
    return span< const typename Container::value_type >( with_container, cont );
}

#endif // ! span_USES_STD_SPAN && span_FEATURE( WITH_CONTAINER )


}  // namespace span_lite
}  // namespace nonstd

// make available in nonstd:

namespace nonstd {
using span_lite::make_span;
}  // namespace nonstd

#endif // #if span_FEATURE_TO_STD( MAKE_SPAN )

#if span_FEATURE( BYTE_SPAN ) && ( span_USES_STD_SPAN || span_HAVE( BYTE ) )

namespace nonstd {
namespace span_lite {

template< class T >
inline span_constexpr auto
byte_span( T & t ) span_noexcept -> span< std::byte, sizeof(t) >
{
    return span< std::byte, sizeof(t) >( reinterpret_cast< std::byte * >( &t ), sizeof(t) );
}

template< class T >
inline span_constexpr auto
byte_span( T const & t ) span_noexcept -> span< const std::byte, sizeof(t) >
{
    return span< const std::byte, sizeof(t) >( reinterpret_cast< std::byte const * >( &t ), sizeof(t) );
}

}  // namespace span_lite
}  // namespace nonstd

// make available in nonstd:

namespace nonstd {
using span_lite::byte_span;
}  // namespace nonstd

#endif // span_FEATURE( BYTE_SPAN )

#if ! span_USES_STD_SPAN
span_RESTORE_WARNINGS()
#endif  // span_USES_STD_SPAN

#endif  // NONSTD_SPAN_HPP_INCLUDED
