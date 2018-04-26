# # handle 
# The MIT License (MIT)
#
# Copyright � 2018 ThePhD
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.5.0)

# # Project declaration
# informs about the project, gives a description, version and MOST IMPORTANTLY
# the languages the project is going to use. Required.
project(handle VERSION 1.0.0 DESCRIPTION "A C++ library that provides an easy and flexible way to work with handle types that may not exactly be pointers." LANGUAGES C CXX)

# # Modules
# Include modules useful to the project, whether locally made in our own cmake DIRECTORY
# our from the standard cmake libraries
# Add home-rolled modules path to front of module path list
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" "${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor/DownloadProject" "${CMAKE_MODULE_PATH}")

# # # Top-Level Directories
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CFG_INT_DIR}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CFG_INT_DIR}/bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CFG_INT_DIR}/bin")
else()
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CFG_INT_DIR}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CFG_INT_DIR}/bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CFG_INT_DIR}/bin")
endif(CMAKE_SIZEOF_VOID_P EQUAL 4)

# # Include standard modules
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(Common/core)

# # Options
#option(HANDLE_CI "Whether or not we are in continguous integration mode" OFF)
#option(HANDLE_TESTS "Enable build of tests" OFF)
#option(HANDLE_BENCHMARKS "Enable build of benchmarks" OFF)

# # Targets
set(handle_sources 
	"include/phd/ebco.hpp"
	"include/phd/meta.hpp"
	"include/phd/handle.hpp"
)
prepend(prefixed_handle_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${handle_sources})

add_library(handle INTERFACE)
add_library(phd::handle ALIAS handle)
target_include_directories(handle INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
	$<INSTALL_INTERFACE:include>
)
target_sources(handle INTERFACE 
	$<BUILD_INTERFACE:${prefixed_handle_sources}>  
	$<INSTALL_INTERFACE:${handle_sources}>
)
set_target_properties(handle PROPERTIES
	EXPORT_NAME phd::handle
)

# # Config / Version packaging
# Version configurations
configure_package_config_file(
	cmake/handle-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/handle-config.cmake"
	INSTALL_DESTINATION lib/cmake/phd/handle
	NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/handle-config-version.cmake"
	COMPATIBILITY AnyNewerVersion)

export(TARGETS handle FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/handle-targets.cmake")

install(TARGETS handle
	EXPORT handle)

install(EXPORT handle
	FILE handle-targets.cmake
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/handle")

install(DIRECTORY "include/phd/handle"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES "include/handle/handle.hpp"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/handle-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/handle-config-version.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/phd/handle"
)

set(HANDLE_TESTS OFF CACHE BOOL "Run the phd::handle library tests")

if (HANDLE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif(HANDLE_TESTS)
