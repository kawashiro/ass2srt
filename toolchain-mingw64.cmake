# toolchain-mingw64.cmake
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# specify the cross compiler
set(CMAKE_C_COMPILER "${MXE_PATH}/usr/bin/i686-w64-mingw32.static-gcc")
set(CMAKE_CXX_COMPILER "${MXE_PATH}/usr/bin/i686-w64-mingw32.static-g++")
set(CMAKE_RC_COMPILER "${MXE_PATH}/usr/bin/i686-w64-mingw32.static-windres")
include_directories(BEFORE SYSTEM "/home/kz/.mxe/usr/i686-w64-mingw32.static/include")

# where is the target environment
set(CMAKE_FIND_ROOT_PATH "${MXE_PATH}/usr/i686-w64-mingw32.static")

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Build static binaries
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
