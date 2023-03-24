# Copyright (c) 2023 Tom Hancocks
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
## Project
project(KDL LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if (USE_LUA_JIT)
    set(KDL_LUA_LIB libluajit.a)
    set(KDL_LUA_DIR ${PROJECT_SUBMODULE_DIR}/LuaJIT/src)
else()
    set(KDL_LUA_LIB Lua)
endif()

########################################################################################################################
## libKDL
file(GLOB_RECURSE libKDL_Sources
    *.cpp
)

add_library(KDL ${libKDL_Sources})
target_link_libraries(KDL SIMD Graphite Foundation Resource Interpreter Lexer Image Sound ${KDL_LUA_LIB})
target_include_directories(KDL PUBLIC
    ${PROJECT_SDK_LIBS}
    ${PROJECT_SUBMODULE_DIR}
    ${PROJECT_SUBMODULE_DIR}/graphite/libs
)

if(USE_LUA_JIT)
    target_link_directories(KDL PUBLIC ${KDL_LUA_DIR})
endif()