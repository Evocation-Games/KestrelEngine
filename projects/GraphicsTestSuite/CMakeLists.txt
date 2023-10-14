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
## Project
project(GraphicsTestSuite LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

########################################################################################################################
## GraphicsTestSuite

if (WIN32)
    set(KDL ${CMAKE_BUILD_DIR}/kdl.exe)
else()
    set(KDL ${CMAKE_BUILD_DIR}/kdl)
endif()

add_custom_target(
    GraphicsTestSuite ALL
        ${KDL} -f extended -o ${CMAKE_BUILD_DIR}/GameCore.kdat
        ${CMAKE_SOURCE_DIR}/projects/GraphicsTestSuite/project.kdlproj
    COMMENT "Building GraphicsTestSuite"
)
add_dependencies(GraphicsTestSuite KDL-Tool)