# Copyright (c) 2022 Tom Hancocks
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

########################################################################################################################
## LuaTool Project Configuration / LLVM

project(kdtool LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if (APPLE)
    execute_process(
        COMMAND zsh -c "/usr/local/bin/brew --prefix llvm@15 | tr -d '\n'"
        OUTPUT_VARIABLE LLVM_DIR
    )
elseif(LINUX)
    set(LLVM_DIR /usr/lib/llvm-14)
elseif(WIN32)
    set(LLVM_DIR D:/a/_temp/msys64/mingw64)
endif()

set(LLVM_INCLUDE_DIRS ${LLVM_DIR}/include)
set(LLVM_LIBS_DIR ${LLVM_DIR}/lib)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,${LLVM_LIBS_DIR}")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

########################################################################################################################
## libClang
if (APPLE)
    add_library(clang SHARED IMPORTED)
    set_property(TARGET clang PROPERTY IMPORTED_LOCATION  "${LLVM_LIBS_DIR}/libclang.dylib")
    set(LIBCLANG_LIBRARY clang)
elseif(LINUX)
    add_library(clang SHARED IMPORTED)
    set_property(TARGET clang PROPERTY IMPORTED_LOCATION "${LLVM_LIBS_DIR}/libclang-14.so")
    set(LIBCLANG_LIBRARY clang)
elseif(WIN32)
    add_library(clang SHARED IMPORTED)
    set_property(TARGET clang PROPERTY IMPORTED_LOCATION "${LLVM_DIR}/bin/libclang.dll")
    set(LIBCLANG_LIBRARY "${LLVM_DIR}/bin/libclang.dll")
endif()

########################################################################################################################
## LuaTool Target

file(GLOB_RECURSE kdtool_Sources src/*.cpp)
add_executable(kdtool ${kdtool_Sources})
set_target_properties(kdtool PROPERTIES OUTPUT_NAME kdtool)
target_link_libraries(kdtool ${LIBCLANG_LIBRARY} CodeGen Foundation)
target_include_directories(kdtool PUBLIC
    ${LLVM_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/src
)

########################################################################################################################
## Installation

install(TARGETS kdtool
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
        DESTINATION ${PROJECT_INSTALL_DIR})