# Copyright (c) 2015, Red Hat, Inc. and/or its affiliates.
#
# This code is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 only, as
# published by the Free Software Foundation.
#
# This code is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# version 2 for more details (a copy is included in the LICENSE file that
# accompanied this code).
#
# You should have received a copy of the GNU General Public License version
# 2 along with this work; if not, write to the Free Software Foundation,
# Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.

# based on https://github.com/madler/zlib/blob/50893291621658f355bc5b4d450a8d06a563053d/CMakeLists.txt
cmake_minimum_required ( VERSION 2.8.12 )

project ( zlib_125 C )

set ( VERSION "1.2.5" )

set ( ${PROJECT_NAME}_SRCDIR ${CMAKE_CURRENT_LIST_DIR}/../../zlib_125 )

if ( NOT ${PROJECT_NAME}_SRCDIR STREQUAL CMAKE_CURRENT_BINARY_DIR )
    # If we're doing an out of source build and the user has a zconf.h
    # in their source tree...
    if ( EXISTS ${${PROJECT_NAME}_SRCDIR}/zconf.h )
        message ( STATUS "Renaming" )
        message ( STATUS "    ${${PROJECT_NAME}_SRCDIR}/zconf.h" )
        message ( STATUS "to 'zconf.h.included' because this file is included with zlib" )
        message ( STATUS "but CMake generates it automatically in the build directory." )
        file ( RENAME ${${PROJECT_NAME}_SRCDIR}/zconf.h ${${PROJECT_NAME}_SRCDIR}/zconf.h.included )
  endif ( )
endif ( )

configure_file(	${${PROJECT_NAME}_SRCDIR}/zconf.h.cmakein
		${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY )

# parse the full version number from zlib.h and include in ZLIB_FULL_VERSION
file(READ ${${PROJECT_NAME}_SRCDIR}/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*"
    "\\1" ZLIB_FULL_VERSION ${_zlib_h_contents})

#============================================================================
# zlib
#============================================================================

set(ZLIB_PUBLIC_HDRS
        ${CMAKE_CURRENT_BINARY_DIR}/zconf.h
        ${${PROJECT_NAME}_SRCDIR}/zlib.h
)
set ( ZLIB_PRIVATE_HDRS
        ${${PROJECT_NAME}_SRCDIR}/crc32.h
        ${${PROJECT_NAME}_SRCDIR}/deflate.h
        ${${PROJECT_NAME}_SRCDIR}/gzguts.h
        ${${PROJECT_NAME}_SRCDIR}/inffast.h
        ${${PROJECT_NAME}_SRCDIR}/inffixed.h
        ${${PROJECT_NAME}_SRCDIR}/inflate.h
        ${${PROJECT_NAME}_SRCDIR}/inftrees.h
        ${${PROJECT_NAME}_SRCDIR}/trees.h
        ${${PROJECT_NAME}_SRCDIR}/zutil.h
)
set ( ZLIB_SRCS
        ${${PROJECT_NAME}_SRCDIR}/adler32.c
        ${${PROJECT_NAME}_SRCDIR}/compress.c
        ${${PROJECT_NAME}_SRCDIR}/crc32.c
        ${${PROJECT_NAME}_SRCDIR}/deflate.c
        ${${PROJECT_NAME}_SRCDIR}/gzclose.c
        ${${PROJECT_NAME}_SRCDIR}/gzlib.c
        ${${PROJECT_NAME}_SRCDIR}/gzread.c
        ${${PROJECT_NAME}_SRCDIR}/gzwrite.c
        ${${PROJECT_NAME}_SRCDIR}/inflate.c
        ${${PROJECT_NAME}_SRCDIR}/infback.c
        ${${PROJECT_NAME}_SRCDIR}/inftrees.c
        ${${PROJECT_NAME}_SRCDIR}/inffast.c
        ${${PROJECT_NAME}_SRCDIR}/trees.c
        ${${PROJECT_NAME}_SRCDIR}/uncompr.c
        ${${PROJECT_NAME}_SRCDIR}/zutil.c
)

add_library ( ${PROJECT_NAME} STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
target_include_directories ( zlib_125 BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${${PROJECT_NAME}_SRCDIR})
if ( MSVC )
    target_compile_definitions ( ${PROJECT_NAME} PRIVATE
            -D_CRT_SECURE_NO_DEPRECATE  
            -D_CRT_NONSTDC_NO_DEPRECATE )
endif ( ) 
