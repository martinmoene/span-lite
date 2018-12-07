# Copyright 2018 by Martin Moene
#
# https://github.com/martinmoene/span-lite
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.5 )

# span-lite project and version, updated by script/update-version.py:

project( span_lite LANGUAGES CXX )

set( span_lite_version "0.3.0" )

# Toplevel or subproject:

if ( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    set( span_IS_TOPLEVEL_PROJECT TRUE )
else()
    set( span_IS_TOPLEVEL_PROJECT FALSE )
endif()

# At default, enable building and performing of tests and building of examples:

option( SPAN_LITE_OPT_BUILD_TESTS    "Build and perform span-lite tests" ${span_IS_TOPLEVEL_PROJECT} )
option( SPAN_LITE_OPT_BUILD_EXAMPLES "Build span-lite examples" OFF )
option( SPAN_LITE_EXPORT_PACKAGE     "Export span-lite package globally" ${span_IS_TOPLEVEL_PROJECT} )

option( SPAN_LITE_COLOURISE_TEST     "Colourise test output" OFF )

option( SPAN_LITE_OPT_SELECT_STD     "Select std::span"    OFF )
option( SPAN_LITE_OPT_SELECT_NONSTD  "Select nonstd::span" OFF )

include( GNUInstallDirs )
include( CMakePackageConfigHelpers )

set( package_name "span-lite" )
set( include_source_dir "${PROJECT_SOURCE_DIR}/include" )

# Interface library:

add_library(
    ${package_name} INTERFACE )

add_library(
    ${package_name}::${package_name} ALIAS ${package_name})

target_include_directories(
    ${package_name} INTERFACE "$<BUILD_INTERFACE:${include_source_dir}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>" )

# Installation:
set(span_lite_config_path "${CMAKE_INSTALL_LIBDIR}/cmake/${package_name}")

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${package_name}ConfigVersion.cmake"
    VERSION ${span_lite_version}
    COMPATIBILITY SameMajorVersion )

# install tree package config
configure_package_config_file(
    span-liteConfig.cmake.in
    ${span_lite_config_path}/span-liteConfig.cmake
    INSTALL_DESTINATION ${span_lite_config_path}
    NO_CHECK_REQUIRED_COMPONENTS_MACRO )

configure_file(
    ${package_name}Config.cmake.in
    ${package_name}Config.cmake
    @ONLY )

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${span_lite_config_path}/${package_name}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${package_name}ConfigVersion.cmake"
    DESTINATION ${span_lite_config_path} )

install(TARGETS ${package_name} EXPORT ${package_name}Targets)

export(
    EXPORT ${package_name}Targets
    NAMESPACE ${package_name}::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${package_name}Targets.cmake" )

install(
    EXPORT ${package_name}Targets
    NAMESPACE ${package_name}::
    FILE ${package_name}Targets.cmake
    DESTINATION ${span_lite_config_path} )

# If requested, export the cmake package to the package registry

if ( SPAN_LITE_EXPORT_PACKAGE )
    export(PACKAGE ${package_name})
endif()

# If requested, build and perform tests, build examples:

enable_testing()

if ( SPAN_LITE_OPT_BUILD_TESTS )
    add_subdirectory( test )
endif()

if ( SPAN_LITE_OPT_BUILD_EXAMPLES )
    add_subdirectory( example )
endif()

# end of file
