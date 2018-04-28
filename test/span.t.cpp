//
// span-lite is based on GSL: Guideline Support Library.
// For more information see https://github.com/martinmoene/span-lite
//
// Copyright (c) 2015 Martin Moene
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "span-main.t.hpp"

#define DIMENSION_OF( a ) ( sizeof(a) / sizeof(0[a]) )

typedef span<int>::index_type index_type;

CASE( "span<>: Terminates construction from a nullptr and a non-zero size (C++11)" )
{
#if span_HAVE( NULLPTR )
    struct F { static void blow() { span<int> v( nullptr, 42 ); } };

    EXPECT_THROWS( F::blow() );
#else
    EXPECT( !!"nullptr is not available (no C++11)" );
#endif
}

CASE( "span<>: Terminates construction from two pointers in the wrong order" )
{
    struct F { static void blow() { int a[2]; span<int> v( &a[1], &a[0] ); } };

    EXPECT_THROWS( F::blow() );
}

CASE( "span<>: Terminates construction from a null pointer and a non-zero size" )
{
    struct F { static void blow() { int * p = span_nullptr; span<int> v( p, 42 ); } };

    EXPECT_THROWS( F::blow() );
}

CASE( "span<>: Terminates creation of a sub span of the first n elements for n exceeding the span" )
{
    struct F {
    static void blow()
    {
        int arr[] = { 1, 2, 3, };
        span<int> v( arr );

        (void) v.first( 4 );
    }};

    EXPECT_THROWS( F::blow() );
}

CASE( "span<>: Terminates creation of a sub span of the last n elements for n exceeding the span" )
{
    struct F {
    static void blow()
    {
        int arr[] = { 1, 2, 3, };
        span<int> v( arr );

        (void) v.last( 4 );
    }};

    EXPECT_THROWS( F::blow() );
}

CASE( "span<>: Terminates creation of a sub span outside the span" )
{
    struct F {
        static void blow_offset() { int arr[] = { 1, 2, 3, }; span<int> v( arr ); (void) v.subspan( 4 ); }
        static void blow_count()  { int arr[] = { 1, 2, 3, }; span<int> v( arr ); (void) v.subspan( 1, 3 ); }
    };

    EXPECT_THROWS( F::blow_offset() );
    EXPECT_THROWS( F::blow_count()  );
}

CASE( "span<>: Terminates access outside the span" )
{
    struct F {
        static void blow_ix(index_type i) { int arr[] = { 1, 2, 3, }; span<int> v( arr ); (void) v[i]; }
        static void blow_iv(index_type i) { int arr[] = { 1, 2, 3, }; span<int> v( arr ); (void) v(i); }
//      static void blow_at(index_type i) { int arr[] = { 1, 2, 3, }; span<int> v( arr ); (void) v.at(i); }
    };

    EXPECT_NO_THROW( F::blow_ix(2) );
    EXPECT_NO_THROW( F::blow_iv(2) );
//  EXPECT_NO_THROW( F::blow_at(2) );
    EXPECT_THROWS(   F::blow_ix(3) );
    EXPECT_THROWS(   F::blow_iv(3) );
//  EXPECT_THROWS(   F::blow_at(3) );
}

CASE( "span<>: Allows to default-construct" )
{
    span<int> v;

    EXPECT( v.size() == index_type( 0 ) );
}

CASE( "span<>: Allows to construct from a nullptr and a zero size (C++11)" )
{
#if span_HAVE( NULLPTR )
    span<      int> v( nullptr, index_type(0) );
    span<const int> w( nullptr, index_type(0) );

    EXPECT( v.size() == index_type( 0 ) );
    EXPECT( w.size() == index_type( 0 ) );
#else
    EXPECT( !!"nullptr is not available (no C++11)" );
#endif
}

CASE( "span<>: Allows to construct from two pointers" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<      int> v( arr, arr + DIMENSION_OF( arr ) );
    span<const int> w( arr, arr + DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
    EXPECT( std::equal( w.begin(), w.end(), arr ) );
}

CASE( "span<>: Allows to construct from two pointers to const" )
{
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<const int> v( arr, arr + DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "span<>: Allows to construct from a non-null pointer and a size" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<      int> v( arr, DIMENSION_OF( arr ) );
    span<const int> w( arr, DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
    EXPECT( std::equal( w.begin(), w.end(), arr ) );
}

CASE( "span<>: Allows to construct from a non-null pointer to const and a size" )
{
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<const int> v( arr, DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "span<>: Allows to construct from a temporary pointer and a size" )
{
    int x = 42;

    span<      int> v( &x, 1 );
    span<const int> w( &x, 1 );

    EXPECT( std::equal( v.begin(), v.end(), &x ) );
    EXPECT( std::equal( w.begin(), w.end(), &x ) );
}

CASE( "span<>: Allows to construct from a temporary pointer to const and a size" )
{
    const int x = 42;

    span<const int> v( &x, 1 );

    EXPECT( std::equal( v.begin(), v.end(), &x ) );
}

CASE( "span<>: Allows to construct from any pointer and a zero size" )
{
    struct F {
        static void null() {
            int * p = span_nullptr; span<int> v( p, index_type( 0 ) );
        }
        static void nonnull() {
            int i = 7; int * p = &i; span<int> v( p, index_type( 0 ) );
        }
    };

    EXPECT_NO_THROW( F::null() );
    EXPECT_NO_THROW( F::nonnull() );
}

CASE( "span<>: Allows to construct from a C-array" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<      int> v( arr );
    span<const int> w( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
    EXPECT( std::equal( w.begin(), w.end(), arr ) );
}

CASE( "span<>: Allows to construct from a const C-array" )
{
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<const int> v( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "span<>: Allows to construct from a C-array with size via decay to pointer (potentially dangerous)" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    {
    span<      int> v( arr, DIMENSION_OF(arr) );
    span<const int> w( arr, DIMENSION_OF(arr) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
    EXPECT( std::equal( w.begin(), w.end(), arr ) );
    }

#if span_CPP14_OR_GREATER
    {
    span<      int> v( arr, 3 );
    span<const int> w( arr, 3 );

    EXPECT( std::equal( v.begin(), v.end(), arr, arr + 3 ) );
    EXPECT( std::equal( w.begin(), w.end(), arr, arr + 3 ) );
    }
#endif
}

CASE( "span<>: Allows to construct from a const C-array with size via decay to pointer (potentially dangerous)" )
{
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    {
    span<const int> v( arr, DIMENSION_OF(arr) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
    }

#if span_CPP14_OR_GREATER
    {
    span<const int> w( arr, 3 );

    EXPECT( std::equal( w.begin(), w.end(), arr, arr + 3 ) );
    }
#endif
}

CASE( "span<>: Allows to construct from a std::array<> (C++11)" )
{
# if span_HAVE( ARRAY )
    std::array<int,9> arr = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, }};

    span<      int> v( arr );
    span<const int> w( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr.begin() ) );
    EXPECT( std::equal( w.begin(), w.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "span<>: Allows to construct from a std::array<> with const data (C++11)" )
{
# if span_HAVE( ARRAY )
    std::array<const int,9> arr = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, }};

    span<const int> v( arr );
    span<const int> const w( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr.begin() ) );
    EXPECT( std::equal( w.begin(), w.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "span<>: Allows to construct from a container (std::vector<>)" )
{
# if span_HAVE( INITIALIZER_LIST )
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    std::vector<int> vec( arr, arr + DIMENSION_OF(arr) );
#endif

#if span_HAVE( CONSTRAINED_SPAN_CONTAINER_CTOR ) || span_HAVE( UNCONSTRAINED_SPAN_CONTAINER_CTOR )
    span<      int> v( vec );
    span<const int> w( vec );

    EXPECT( std::equal( v.begin(), v.end(), vec.begin() ) );
    EXPECT( std::equal( w.begin(), w.end(), vec.begin() ) );
#else
    EXPECT( !!"(un)constrained construction from container is not available" );
#endif
}

CASE( "span<>: Allows to tag-construct from a container (std::vector<>)" )
{
# if span_HAVE( INITIALIZER_LIST )
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    std::vector<int> vec( arr, arr + DIMENSION_OF(arr) );
#endif
    span<      int> v( with_container, vec );
    span<const int> w( with_container, vec );

    EXPECT( std::equal( v.begin(), v.end(), vec.begin() ) );
    EXPECT( std::equal( w.begin(), w.end(), vec.begin() ) );
}

CASE( "span<>: Allows to tag-construct from a const container (std::vector<>)" )
{
# if span_HAVE( INITIALIZER_LIST )
    const std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    const std::vector<int> vec( arr, arr + DIMENSION_OF(arr) );
#endif
    span<      int> v( with_container, vec );
    span<const int> w( with_container, vec );

    EXPECT( std::equal( v.begin(), v.end(), vec.begin() ) );
    EXPECT( std::equal( w.begin(), w.end(), vec.begin() ) );
}

CASE( "span<>: Allows to copy-construct from another span of the same type" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    span<      int> v( arr );
    span<const int> w( arr );

    span<      int> x( v );
    span<const int> y( w );

    EXPECT( std::equal( x.begin(), x.end(), arr ) );
    EXPECT( std::equal( y.begin(), y.end(), arr ) );
}

CASE( "span<>: Allows to copy-construct from another span of a compatible type" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    span<      int> v( arr );
    span<const int> w( arr );

    span<const volatile int> x( v );
    span<const volatile int> y( v );

    EXPECT( std::equal( x.begin(), x.end(), arr ) );
    EXPECT( std::equal( y.begin(), y.end(), arr ) );
}

CASE( "span<>: Allows to copy-construct from a temporary span of the same type (C++11)" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<      int> v(( span<      int>( arr ) ));
//  span<      int> w(( span<const int>( arr ) ));
    span<const int> x(( span<      int>( arr ) ));
    span<const int> y(( span<const int>( arr ) ));

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
    EXPECT( std::equal( x.begin(), x.end(), arr ) );
    EXPECT( std::equal( y.begin(), y.end(), arr ) );
}

CASE( "span<>: Allows to copy-assign from another span of the same type" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    span<      int> v( arr );
    span<      int> s;
    span<const int> t;

    s = v;
    t = v;

    EXPECT( std::equal( s.begin(), s.end(), arr ) );
    EXPECT( std::equal( t.begin(), t.end(), arr ) );
}

CASE( "span<>: Allows to copy-assign from a temporary span of the same type (C++11)" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
    span<      int> v;
    span<const int> w;

    v = span<int>( arr );
    w = span<int>( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
    EXPECT( std::equal( w.begin(), w.end(), arr ) );
}

CASE( "span<>: Allows to create a sub span of the first n elements" )
{
    int arr[] = { 1, 2, 3, 4, 5, };
    span<int> v( arr );
    index_type count = 3;

    span<      int> s = v.first( count );
    span<const int> t = v.first( count );

    EXPECT( s.size() == count );
    EXPECT( t.size() == count );
    EXPECT( std::equal( s.begin(), s.end(), arr ) );
    EXPECT( std::equal( t.begin(), t.end(), arr ) );
}

CASE( "span<>: Allows to create a sub span of the last n elements" )
{
    int arr[] = { 1, 2, 3, 4, 5, };
    span<int> v( arr );
    index_type count = 3;

    span<      int> s = v.last( count );
    span<const int> t = v.last( count );

    EXPECT( s.size() == count );
    EXPECT( t.size() == count );
    EXPECT( std::equal( s.begin(), s.end(), arr + v.size() - count ) );
    EXPECT( std::equal( t.begin(), t.end(), arr + v.size() - count ) );
}

CASE( "span<>: Allows to create a sub span starting at a given offset" )
{
    int arr[] = { 1, 2, 3, };
    span<int> v( arr );
    index_type offset = 1;

    span<      int> s = v.subspan( offset );
    span<const int> t = v.subspan( offset );

    EXPECT( s.size() == v.size() - offset );
    EXPECT( t.size() == v.size() - offset );
    EXPECT( std::equal( s.begin(), s.end(), arr + offset ) );
    EXPECT( std::equal( t.begin(), t.end(), arr + offset ) );
}

CASE( "span<>: Allows to create a sub span starting at a given offset with a given length" )
{
    int arr[] = { 1, 2, 3, };
    span<int> v( arr );
    index_type offset = 1;
    index_type length = 1;

    span<      int> s = v.subspan( offset, length );
    span<const int> t = v.subspan( offset, length );

    EXPECT( s.size() == length );
    EXPECT( t.size() == length );
    EXPECT( std::equal( s.begin(), s.end(), arr + offset ) );
    EXPECT( std::equal( t.begin(), t.end(), arr + offset ) );
}

//CASE( "span<>: Allows to create an empty sub span at full offset" )
//{
//    int arr[] = { 1, 2, 3, };
//    span<int> v( arr );
//    index_type offset = v.size() - 1;
//
//    span<      int> s = v.subspan( offset );
//    span<const int> t = v.subspan( offset );
//
//    EXPECT( s.empty() );
//    EXPECT( t.empty() );
//}

//CASE( "span<>: Allows to create an empty sub span at full offset with zero length" )
//{
//    int arr[] = { 1, 2, 3, };
//    span<int> v( arr );
//    index_type offset = v.size();
//    index_type length = 0;
//
//    span<      int> s = v.subspan( offset, length );
//    span<const int> t = v.subspan( offset, length );
//
//    EXPECT( s.empty() );
//    EXPECT( t.empty() );
//}

CASE( "span<>: Allows forward iteration" )
{
    int arr[] = { 1, 2, 3, };
    span<int> v( arr );

    for ( span<int>::iterator pos = v.begin(); pos != v.end(); ++pos )
    {
        EXPECT( *pos == arr[ std::distance( v.begin(), pos )] );
    }
}

CASE( "span<>: Allows const forward iteration" )
{
    int arr[] = { 1, 2, 3, };
    span<int> v( arr );

    for ( span<int>::const_iterator pos = v.cbegin(); pos != v.cend(); ++pos )
    {
        EXPECT( *pos == arr[ std::distance( v.cbegin(), pos )] );
    }
}

CASE( "span<>: Allows reverse iteration" )
{
    int arr[] = { 1, 2, 3, };
    span<int> v( arr );

    for ( span<int>::reverse_iterator pos = v.rbegin(); pos != v.rend(); ++pos )
    {
//        size_t dist = narrow<size_t>( std::distance(v.rbegin(), pos) );
        index_type dist = std::distance(v.rbegin(), pos);
        EXPECT( *pos == arr[ v.size() - 1 - dist ] );
    }
}

CASE( "span<>: Allows const reverse iteration" )
{
    int arr[] = { 1, 2, 3, };
    const span<int> v( arr );

    for ( span<int>::const_reverse_iterator pos = v.crbegin(); pos != v.crend(); ++pos )
    {
//        size_t dist = narrow<size_t>( std::distance(v.crbegin(), pos) );
        index_type dist = std::distance(v.crbegin(), pos);
        EXPECT( *pos == arr[ v.size() - 1 - dist ] );
    }
}

CASE( "span<>: Allows to observe an element via array indexing" )
{
    int arr[] = { 1, 2, 3, };
    span<int>       v( arr );
    span<int> const w( arr );

    for ( index_type i = 0; i < v.size(); ++i )
    {
        EXPECT( v[i] == arr[i] );
        EXPECT( w[i] == arr[i] );
    }
}

CASE( "span<>: Allows to observe an element via call indexing" )
{
    int arr[] = { 1, 2, 3, };
    span<int>       v( arr );
    span<int> const w( arr );

    for ( index_type i = 0; i < v.size(); ++i )
    {
        EXPECT( v(i) == arr[i] );
        EXPECT( w(i) == arr[i] );
    }
}

CASE( "span<>: Allows to observe an element via data()" )
{
    int arr[] = { 1, 2, 3, };
    span<int>       v( arr );
    span<int> const w( arr );

    EXPECT( *v.data() == *v.begin() );
    EXPECT( *w.data() == *v.begin() );

    for ( index_type i = 0; i < v.size(); ++i )
    {
        EXPECT( v.data()[i] == arr[i] );
        EXPECT( w.data()[i] == arr[i] );
    }
}

CASE( "span<>: Allows to change an element via array indexing" )
{
    int arr[] = { 1, 2, 3, };
    span<int>       v( arr );
    span<int> const w( arr );

    v[1] = 22;
    w[2] = 33;

    EXPECT( 22 == arr[1] );
    EXPECT( 33 == arr[2] );
}

CASE( "span<>: Allows to change an element via call indexing" )
{
    int arr[] = { 1, 2, 3, };
    span<int>       v( arr );
    span<int> const w( arr );

    v(1) = 22;
    w(2) = 33;

    EXPECT( 22 == arr[1] );
    EXPECT( 33 == arr[2] );
}

CASE( "span<>: Allows to change an element via data()" )
{
    int arr[] = { 1, 2, 3, };

    span<int> v( arr );
    span<int> const w( arr );

    *v.data() = 22;
    EXPECT( 22 == *v.data() );

    *w.data() = 33;
    EXPECT( 33 == *w.data() );
}

CASE( "span<>: Allows to compare equal to another span of the same type" )
{
    int a[] = { 1 }, b[] = { 2 }, c[] = { 1, 2 };
    span<int> va( a );
    span<int> vb( b );
    span<int> vc( c );

    EXPECT(     va == va );
    EXPECT_NOT( vb == va );
    EXPECT_NOT( vc == va );
}

CASE( "span<>: Allows to compare unequal to another span of the same type" )
{
    int a[] = { 1 }, b[] = { 2 }, c[] = { 1, 2 };
    span<int> va( a );
    span<int> vb( b );
    span<int> vc( c );

    EXPECT_NOT( va != va );
    EXPECT(     vb != va );
    EXPECT(     vc != va );
}

CASE( "span<>: Allows to compare less than another span of the same type" )
{
    int a[] = { 1 }, b[] = { 2 }, c[] = { 1, 2 };
    span<int> va( a );
    span<int> vb( b );
    span<int> vc( c );

    EXPECT_NOT( va < va );
    EXPECT(     va < vb );
    EXPECT(     va < vc );
}

CASE( "span<>: Allows to compare less than or equal to another span of the same type" )
{
    int a[] = { 1 }, b[] = { 2 }, c[] = { 1, 2 };
    span<int> va( a );
    span<int> vb( b );
    span<int> vc( c );

    EXPECT_NOT( vb <= va );
    EXPECT(     va <= vb );
    EXPECT(     va <= vc );
}

CASE( "span<>: Allows to compare greater than another span of the same type" )
{
    int a[] = { 1 }, b[] = { 2 }, c[] = { 1, 2 };
    span<int> va( a );
    span<int> vb( b );
    span<int> vc( c );

    EXPECT_NOT( va > va );
    EXPECT(     vb > va );
    EXPECT(     vc > va );
}

CASE( "span<>: Allows to compare greater than or equal to another span of the same type" )
{
    int a[] = { 1 }, b[] = { 2 }, c[] = { 1, 2 };
    span<int> va( a );
    span<int> vb( b );
    span<int> vc( c );

    EXPECT_NOT( va >= vb );
    EXPECT(     vb >= va );
    EXPECT(     vc >= va );
}

CASE( "span<>: Allows to compare to another span of the same type and different cv-ness (non-standard)" )
{
#if span_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON
    int aa[] = { 1 }, bb[] = { 2 };
    span<         int>  a( aa );
    span<   const int> ca( aa );
    span<volatile int> va( aa );
    span<         int>  b( bb );
    span<   const int> cb( bb );

    EXPECT( va == ca );
    EXPECT(  a == va );

    EXPECT(  a == ca );
    EXPECT(  a != cb );
    EXPECT(  a <= cb );
    EXPECT(  a <  cb );
    EXPECT(  b >= ca );
    EXPECT(  b >  ca );
#else
    EXPECT( !!"span<>: cannot compare different types (span_CONFIG_ALLOWS_NONSTRICT_SPAN_COMPARISON=0)" );
#endif
}

CASE( "span<>: Allows to compare empty spans as equal" )
{
    int a;

    span<int> p;
    span<int> q;
    span<int> r( &a, index_type( 0 ) );

    EXPECT( p == q );
    EXPECT( p == r );

#if span_HAVE( NULLPTR )
    span<int> s( nullptr, index_type( 0 ) );
    span<int> t( nullptr, index_type( 0 ) );

    EXPECT( s == p );
    EXPECT( s == r );
    EXPECT( s == t );
#endif
}

CASE( "span<>: Allows to test for empty span via empty(), empty case" )
{
    span<int> v;

    EXPECT( v.empty() );
}

CASE( "span<>: Allows to test for empty span via empty(), non-empty case" )
{
    int a[] = { 1 };
    span<int> v( a );

    EXPECT_NOT( v.empty() );
}

CASE( "span<>: Allows to obtain the number of elements via size()" )
{
    int a[] = { 1, 2, 3, };
    int b[] = { 1, 2, 3, 4, 5, };

    span<int> z;
    span<int> va( a );
    span<int> vb( b );

    EXPECT( va.size() == index_type( DIMENSION_OF( a ) ) );
    EXPECT( vb.size() == index_type( DIMENSION_OF( b ) ) );
    EXPECT(  z.size() == index_type( 0 ) );
}

CASE( "span<>: Allows to obtain the number of bytes via size_bytes()" )
{
    int a[] = { 1, 2, 3, };
    int b[] = { 1, 2, 3, 4, 5, };

    span<int> z;
    span<int> va( a );
    span<int> vb( b );

    EXPECT( va.size_bytes() == index_type( DIMENSION_OF( a ) * sizeof(int) ) );
    EXPECT( vb.size_bytes() == index_type( DIMENSION_OF( b ) * sizeof(int) ) );
    EXPECT(  z.size_bytes() == index_type( 0 * sizeof(int) ) );
}

//CASE( "span<>: Allows to swap with another span of the same type" )
//{
//    int a[] = { 1, 2, 3, };
//    int b[] = { 1, 2, 3, 4, 5, };
//
//    span<int> va0( a );
//    span<int> vb0( b );
//    span<int> va( a );
//    span<int> vb( b );
//
//    va.swap( vb );
//
//    EXPECT( va == vb0 );
//    EXPECT( vb == va0 );
//}

#if span_HAVE( BYTE )

static bool is_little_endian()
{
    union U
    {
        U() : i(1) {}
        int i;
        char c[ sizeof(int) ];
    };
    return 1 != U().c[ sizeof(int) - 1 ];
}
#endif

CASE( "span<>: Allows to view the elements as read-only bytes" )
{
#if span_HAVE( BYTE )
    using std::byte;
    using type = std::int32_t;

    EXPECT( sizeof( type ) == size_t( 4 ) );

    type  a[] = { 0x12345678, };
    byte be[] = { byte{0x12}, byte{0x34}, byte{0x56}, byte{0x78}, };
    byte le[] = { byte{0x78}, byte{0x56}, byte{0x34}, byte{0x12}, };

    byte * b = is_little_endian() ? le : be;

    span<type> va( a );
    span<const byte> vb( as_bytes( va ) );

    EXPECT( vb[0] == b[0] );
    EXPECT( vb[1] == b[1] );
    EXPECT( vb[2] == b[2] );
    EXPECT( vb[3] == b[3] );
#else
    EXPECT( !!"std::byte is not available (no C++17)" );
#endif
}

CASE( "span<>: Allows to view and change the elements as writable bytes" )
{
#if span_HAVE( BYTE )
    using std::byte;
    using type = std::int32_t;

    EXPECT( sizeof(type) == size_t( 4 ) );

    type  a[] = { 0x0, };
    span<type> va( a );
    span<byte> vb( as_writeable_bytes(va) );

    for ( size_t i = 0; i < sizeof(type); ++i )
    {
        EXPECT( vb[i] == byte{0} );
    }

    vb[0] = byte{0x42};

    EXPECT( vb[0] == byte{0x42} );
    for ( size_t i = 1; i < sizeof(type); ++i )
    {
        EXPECT( vb[i] == byte{0} );
    }
#else
    EXPECT( !!"std::byte is not available (no C++17)" );
#endif
}

//CASE( "span<>: Allows to view the elements as a span of another type" )
//{
//#if span_HAVE( SIZED_TYPES )
//    typedef int32_t type1;
//    typedef int16_t type2;
//#else
//    typedef int   type1;
//    typedef short type2;
//#endif
//    EXPECT( sizeof( type1 ) == size_t( 4 ) );
//    EXPECT( sizeof( type2 ) == size_t( 2 ) );
//
//    type1  a[] = { 0x12345678, };
//    type2 be[] = { type2(0x1234), type2(0x5678), };
//    type2 le[] = { type2(0x5678), type2(0x1234), };
//
//    type2 * b = is_little_endian() ? le : be;
//
//    span<type1> va( a );
//    span<type2> vb( va );
//
//    EXPECT( vb[0] == b[0] );
//    EXPECT( vb[1] == b[1] );
//}

//CASE( "span<>: Allows to change the elements from a span of another type" )
//{
//#if span_HAVE( SIZED_TYPES )
//    typedef int32_t type1;
//    typedef int16_t type2;
//#else
//    typedef int   type1;
//    typedef short type2;
//#endif
//    EXPECT( sizeof( type1 ) == size_t( 4 ) );
//    EXPECT( sizeof( type2 ) == size_t( 2 ) );
//
//    type1  a[] = { 0x0, };
//
//    span<type1> va( a );
//#if span_COMPILER_MSVC_VERSION == 6
//    span<type2> vb( va.as_span( type2() ) );
//#else
//    span<type2> vb( va.as_span<type2>() );
//#endif
//
//    {for ( size_t i = 0; i < sizeof(type2); ++i ) EXPECT( vb[i] == type2(0) ); }
//
//    vb[0] = 0x42;
//
//    EXPECT( vb[0] == type2(0x42) );
//    {for ( size_t i = 1; i < sizeof(type2); ++i ) EXPECT( vb[i] == type2(0) ); }
//}

#if span_CONFIG_PROVIDE_MAKE_SPAN

CASE( "make_span(): Allows building from two pointers" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<int> v = make_span( arr, arr + DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "make_span(): Allows building from two const pointers" )
{
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<const int> v = make_span( arr, arr + DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "make_span(): Allows building from a non-null pointer and a size" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<int> v = make_span( arr, DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "make_span(): Allows building from a non-null const pointer and a size" )
{
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<const int> v = make_span( arr, DIMENSION_OF( arr ) );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "make_span(): Allows building from a C-array" )
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<int> v = make_span( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "make_span(): Allows building from a const C-array" )
{
    const int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

    span<const int> v = make_span( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr ) );
}

CASE( "make_span(): Allows building from a std::array<> (C++11)" )
{
# if span_HAVE( ARRAY )
    std::array<int,9> arr = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, }};

    span<int> v = make_span( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "make_span(): Allows building from a const std::array<> (C++11)" )
{
# if span_HAVE( ARRAY )
    const std::array<int,9> arr = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, }};

    span<const int> v = make_span( arr );

    EXPECT( std::equal( v.begin(), v.end(), arr.begin() ) );
#else
    EXPECT( !!"std::array<> is not available (no C++11)" );
#endif
}

CASE( "make_span(): Allows building from a container (std::vector<>)" )
{
# if span_HAVE( INITIALIZER_LIST )
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    std::vector<int> vec; {for ( int i = 1; i < 10; ++i ) vec.push_back(i); }
#endif
    span<int> v = make_span( vec );

    EXPECT( std::equal( v.begin(), v.end(), vec.begin() ) );
}

CASE( "make_span(): Allows building from a const container (std::vector<>)" )
{
# if span_HAVE( INITIALIZER_LIST )
    const std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
#else
    const std::vector<int> vec( 10, 42 );
#endif
    span<const int> v = make_span( vec );

    EXPECT( std::equal( v.begin(), v.end(), vec.begin() ) );
}

#endif // span_CONFIG_PROVIDE_MAKE_SPAN

// end of file
