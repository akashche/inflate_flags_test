Utility for testing zlib's inflate
==================================

This project contains a small utility `inflate_flags_test` that allows to test a behaviour
of `inflate` function with different options and on different versions of a [zlib](https://github.com/madler/zlib)
library. It will decompress the test ZIP entry using one or more `inflate` calls and will
exit without calling `inflateEnd` to emulate a real-life situation when ZIP files are left opened.

This project contains multiple versions of a zlib as git submodules pinned to the different zlib tags.

Compiled version of the [XSDHandler](http://hg.openjdk.java.net/jdk7u/jdk7u/jaxp/file/b5c74ec32065/src/com/sun/org/apache/xerces/internal/impl/xs/traversers/XSDHandler.java)
class from jdk7u is used as a test ZIP file.

How to build
------------

[CMake](http://cmake.org/) is required for building.

    git clone --recursive https://github.com/akashche/inflate_flags_test.git
    cd inflate_flags_test
    mkdir build
    cd build
    cmake ..
    # on Linux and Mac
    make
    # on Windows
    msbuild inflate_flags_test.sln

Multiple `inflate_flags_test_XXX` executables will be built, each one is linked statically with 
the corresponding included version of a zlib.

Note, message `Renaming ... to 'zconf.h.included' because this file is included with zlib` will be
displayed on a first build - this bit was taken from the official zlib's cmake script as-is.

How to run
----------

`inflate_flags_test` utility takes the following command line arguments:

 - zlib's `inflate` flush mode as an integer (required)
 - size of the input buffer specified to the `inflate` (optional)
 - size of the output buffer specified to the `inflate` (optional)

Zlib's `inflate` flush modes:

 - `Z_NO_FLUSH (0)`
 - `Z_PARTIAL_FLUSH (1)`
 - `Z_SYNC_FLUSH (2)`
 - `Z_FULL_FLUSH (3)`
 - `Z_FINISH (4)`
 - `Z_BLOCK (5)`
 - `Z_TREES (6)`

By default input and output buffers for `inflate` call will be big enough to decompress the whole
ZIP entry in one pass.

How to check inflate's memory usage
-----------------------------------

On Linux and Mac `inflate`'s memory usage can be detected using [Memcheck](http://valgrind.org/docs/manual/mc-manual.html)
tool from a Valgrind:

    valgrind \
        --tool=memcheck \
        --leak-check=yes \
        --show-reachable=yes \
        ./inflate_flags_test_123 1 8192 8192        

One memory leak from `inflateInit2_` call will always be present (will be of different size for different zlib versions):

    7,152 bytes in 1 blocks are definitely lost in loss record 1 of 1
       at 0x4C28BED: malloc (vg_replace_malloc.c:263)
       by 0x405A0F: zcalloc (in ...)
       by 0x401925: inflateInit2_ (in ...)
       by 0x4015C4: main (in ...)

The second leak from the `inflate` -> `updatewindow` call will be present is some cases 
(previous leak will also show it as an "indirect one"):

    32,768 bytes in 1 blocks are indirectly lost in loss record 1 of 2
       at 0x4C28BED: malloc (vg_replace_malloc.c:263)
       by 0x405A0F: zcalloc (in ...)
       by 0x401B27: updatewindow (in ...)
       by 0x403EDD: inflate (in ...)
       by 0x40140F: do_inflate (in ...)
       by 0x4015EC: main (in ...)

On Windows [Dr. Memory](http://www.drmemory.org/) tool can be used instead of a Valgrind:

    drmemory ^
        -show_reachable ^
        -batch ^
        -- ^
        inflate_flags_test_123.exe 1 8192 8192

Dr. Memory won't show the second leak with detailed stack trace as a Valgrind, but still can show it
as an "indirect" part of the first leak:

    Error #7: LEAK 7116 direct bytes 0x015f9dd8-0x015fb9a4 + 32768 indirect bytes
    # 0 replace_malloc               [d:\drmemory_package\common\alloc_replace.c:2537]
    # 1 zcalloc                      [...]
    # 2 inflateInit2_                [...]
    # 3 main                         [...]


License information
-------------------

This project is released under the [GNU General Public License, version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).

Changelog
---------

**2015-11-02**

 * initial public version
