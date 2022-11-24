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

project(LuaTool LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    execute_process(
        COMMAND zsh-c "brew --prefix llvm"
        OUPUT_VARIABLE LLVM_DIR
    )
    message("LLVM_DIR = ${LLVM_DIR}")
endif()

set(LLVM_INCLUDE_DIR ${LLVM_DIR}/include)
set(LLVM_LIBS_DIR ${LLVM_DIR}/lib)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,${LLVM_LIBS_DIR}")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

########################################################################################################################
## libClang

if (APPLE)
    add_library(clang SHARED IMPORTED)
    set_property(TARGET clang PROPERTY IMPORTED_LOCATION "${LLVM_LIBS_DIR}/libclang.dylib")
endif()

########################################################################################################################
## LuaTool Target

file(GLOB_RECURSE LuaToolSources
    src/*.cpp
)

add_executable(LuaTool ${LuaToolSources})
set_target_properties(LuaTool PROPERTIES OUTPUT_NAME luatool)
target_link_libraries(LuaTool clang)
target_include_directories(LuaTool PUBLIC
    ${LLVM_INCLUDE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/src
)