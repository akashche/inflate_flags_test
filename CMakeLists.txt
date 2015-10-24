
cmake_minimum_required ( VERSION 2.8.12 )

project ( inflate_flags_test C )

include ( ${CMAKE_CURRENT_LIST_DIR}/resources/cmake_macros/output_dirs.cmake )
include ( ${CMAKE_CURRENT_LIST_DIR}/resources/cmake_macros/zlib_checks.cmake )

add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/resources/zlib_128_cmake ${CMAKE_CURRENT_BINARY_DIR}/zlib_128 )

add_executable ( ${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/inflate_flags_test.c )
target_link_libraries ( ${PROJECT_NAME} zlib_128 )
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/zlib_128
        ${CMAKE_CURRENT_LIST_DIR}/zlib_128 )
target_compile_options ( ${PROJECT_NAME} PRIVATE -Wall -Werror -Wextra )
