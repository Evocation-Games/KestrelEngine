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

cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

########################################################################################################################
## Project Configuration

project(RenderTest LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

########################################################################################################################
## Compilation
file(GLOB_RECURSE RenderTest_Sources
    *.cpp
)

add_executable(RenderTest ${RenderTest_Sources})
target_include_directories(RenderTest PUBLIC
    ${PROJECT_SDK_LIBS}
    ${PROJECT_SDK_BIN}/render-test
)

if(APPLE)
    target_link_libraries(RenderTest Foundation Image RenderCore MetalRenderer OpenGLRenderer)
else()
    target_link_libraries(RenderTest Foundation Image RenderCore OpenGLRenderer)
endif()

