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

cmake_minimum_required ( VERSION 2.8.12 )

project ( inflate_flags_test C )
include ( ${CMAKE_CURRENT_LIST_DIR}/resources/cmake_macros/output_dirs.cmake )
include ( ${CMAKE_CURRENT_LIST_DIR}/resources/cmake_macros/zlib_checks.cmake )

# zlib v1.2.3
add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/resources/zlib_123_cmake ${CMAKE_CURRENT_BINARY_DIR}/zlib_123 )
add_executable ( ${PROJECT_NAME}_123 ${CMAKE_CURRENT_LIST_DIR}/src/inflate_flags_test.c )
target_link_libraries ( ${PROJECT_NAME}_123 zlib_123 )
target_include_directories ( ${PROJECT_NAME}_123 BEFORE PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/zlib_123
        ${CMAKE_CURRENT_LIST_DIR}/zlib_123 )
if ( MSVC )
    target_compile_definitions ( ${PROJECT_NAME}_123 PRIVATE -D_CRT_SECURE_NO_WARNINGS )
endif ( )

# zlib v1.2.5
add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/resources/zlib_125_cmake ${CMAKE_CURRENT_BINARY_DIR}/zlib_125 )
add_executable ( ${PROJECT_NAME}_125 ${CMAKE_CURRENT_LIST_DIR}/src/inflate_flags_test.c )
target_link_libraries ( ${PROJECT_NAME}_125 zlib_125 )
target_include_directories ( ${PROJECT_NAME}_125 BEFORE PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/zlib_125
        ${CMAKE_CURRENT_LIST_DIR}/zlib_125 )
if ( MSVC )
    target_compile_definitions ( ${PROJECT_NAME}_125 PRIVATE -D_CRT_SECURE_NO_WARNINGS )
endif ( )

# zlib v.1.2.8
add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/resources/zlib_128_cmake ${CMAKE_CURRENT_BINARY_DIR}/zlib_128 )
add_executable ( ${PROJECT_NAME}_128 ${CMAKE_CURRENT_LIST_DIR}/src/inflate_flags_test.c )
target_link_libraries ( ${PROJECT_NAME}_128 zlib_128 )
target_include_directories ( ${PROJECT_NAME}_128 BEFORE PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/zlib_128
        ${CMAKE_CURRENT_LIST_DIR}/zlib_128 )
if ( MSVC )
    target_compile_definitions ( ${PROJECT_NAME}_128 PRIVATE -D_CRT_SECURE_NO_WARNINGS )
endif ( )
