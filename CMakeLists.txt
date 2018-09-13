# Copyright 2018 by Martin Moene
#
# https://github.com/martinmoene/span-lite
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.5 )

# span-lite version, updated by script/update-version.py:

set( span_lite_version "0.3.0" )

# At default, enable building and performing of tests and building of examples:

option( SPAN_LITE_OPT_BUILD_TESTS    "Build and perform span-lite tests" ON )
option( SPAN_LITE_OPT_BUILD_EXAMPLES "Build span-lite examples" OFF )

option( RING_SPAN_LITE_COLOURISE_TEST "Colourise test output" OFF )

project( span_lite LANGUAGES CXX )

include( GNUInstallDirs )

set( package_name "span-lite" )
set( include_source_dir "${PROJECT_SOURCE_DIR}/include" )

# Interface library:

add_library(                
    ${package_name} INTERFACE )
    
target_include_directories( 
    ${package_name} INTERFACE "$<BUILD_INTERFACE:${include_source_dir}>" )

# Installation:

install( 
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

# If requested, build and perform tests, build examples:

enable_testing()

if ( SPAN_LITE_OPT_BUILD_TESTS )
    add_subdirectory( test )
endif()

if ( SPAN_LITE_OPT_BUILD_EXAMPLES )
    add_subdirectory( example )
endif()

# end of file
