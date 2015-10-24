
cmake_minimum_required ( VERSION 2.8.12 )

include ( CheckTypeSize )
include ( CheckFunctionExists )
include ( CheckIncludeFile )
include ( CheckCSourceCompiles )

check_include_file ( sys/types.h HAVE_SYS_TYPES_H )
check_include_file ( stdint.h    HAVE_STDINT_H )
check_include_file ( stddef.h    HAVE_STDDEF_H )

#
# Check to see if we have large file support
#
set ( CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1 )
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif()
if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif()
if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif()
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)