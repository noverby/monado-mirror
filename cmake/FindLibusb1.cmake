# - try to find libusb-1 library
#
# Cache Variables: (probably not for direct use in your scripts)
#  LIBUSB1_LIBRARY
#  LIBUSB1_INCLUDE_DIR
#
# Non-cache variables you should use in your CMakeLists.txt:
#  LIBUSB1_LIBRARIES
#  LIBUSB1_INCLUDE_DIRS
#  LIBUSB1_FOUND - if this is not true, do not attempt to use this library
#
# Requires these CMake modules:
#  ProgramFilesGlob
#  FindPackageHandleStandardArgs (known included with CMake >=2.6.2)
#
# Original Author:
# 2009-2021 Rylie Pavlik <rylie@ryliepavlik.com>
# https://ryliepavlik.com/
#
# Copyright 2009-2010, Iowa State University
# Copyright 2021, Collabora, Ltd.
#
# SPDX-License-Identifier: BSL-1.0
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)


set(LIBUSB1_ROOT_DIR
	"${LIBUSB1_ROOT_DIR}"
	CACHE
	PATH
	"Root directory to search for libusb-1")

if(WIN32)
	include(ProgramFilesGlob)
	program_files_fallback_glob(_dirs "LibUSB-Win32")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		if(MSVC)
			set(_lib_suffixes lib/msvc_x64 MS64/static)
		endif()
	else()
		if(MSVC)
			set(_lib_suffixes lib/msvc MS32/static)
		elseif(COMPILER_IS_GNUCXX)
			set(_lib_suffixes lib/gcc)
		endif()
	endif()
else()
	set(_lib_suffixes)
	if(NOT ANDROID)
		find_package(PkgConfig QUIET)
		if(PKG_CONFIG_FOUND)
			pkg_check_modules(PC_LIBUSB1 QUIET libusb-1.0)
		endif()
	endif()
endif()

find_path(LIBUSB1_INCLUDE_DIR
	NAMES
	libusb.h
	PATHS
	${PC_LIBUSB1_INCLUDE_DIRS}
	${PC_LIBUSB1_INCLUDEDIR}
	${_dirs}
	HINTS
	"${LIBUSB1_ROOT_DIR}"
	PATH_SUFFIXES
	include/libusb-1.0
	include
	libusb-1.0)

find_library(LIBUSB1_LIBRARY
	NAMES
	libusb-1.0
	usb-1.0
	PATHS
	${PC_LIBUSB1_LIBRARY_DIRS}
	${PC_LIBUSB1_LIBDIR}
	${_dirs}
	HINTS
	"${LIBUSB1_ROOT_DIR}"
	PATH_SUFFIXES
	${_lib_suffixes})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libusb1
	DEFAULT_MSG
	LIBUSB1_LIBRARY
	LIBUSB1_INCLUDE_DIR)

if(LIBUSB1_FOUND)
	set(LIBUSB1_LIBRARIES "${LIBUSB1_LIBRARY}")

	set(LIBUSB1_INCLUDE_DIRS "${LIBUSB1_INCLUDE_DIR}")

	mark_as_advanced(LIBUSB1_ROOT_DIR)
endif()

mark_as_advanced(LIBUSB1_INCLUDE_DIR LIBUSB1_LIBRARY)
