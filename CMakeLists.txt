# Copyright (c) 2021 Tom Hancocks
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

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
endif()

########################################################################################################################
## Kestrel Game Engine Project Configuration
project(KestrelGameEngine LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(KESTREL_PLAYER_NAME "Kestrel" CACHE STRING "Kestrel Package Name")
set(KESTREL_PLAYER_BIN_NAME "kestrel" CACHE STRING "Kestrel Binary Name")
set(KESTREL_PLAYER_ICON_NAME "icon.png" CACHE STRING "Kestrel Packaged Icon Name")
set(KESTREL_PLAYER_ICON_PATH "support/packaging/icon-1024.png" CACHE STRING "Kestrel Icon Path")
set(KESTREL_PLAYER_PROJECT_PATH "projects/GraphicsTestSuite/project.kdlproj" CACHE STRING "Kestrel Project Path")
set(KESTREL_PLAYER_DATA_FILES_PATH "build/Scenario" CACHE STRING "Kestrel Data Files Path")
set(KESTREL_VERSION "0.9.1" CACHE STRING "Kestrel Version")
set(KESTREL_BUILD_NUMBER "1" CACHE STRING "Kestrel Build Number")

if (APPLE)
    set(OSX_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(APPLE_SILICON 1)
        set(BREW "/opt/homebrew/bin/brew")
    else()
        set(BREW "/usr/local/bin/brew")
    endif()
elseif (UNIX)
    set(LINUX TRUE)
endif()

if (NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -msse -msse4.1")
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

if(CMAKE_BUILD_TYPE MATCHES ASan)
    message("Compiling with Address Sanitizer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR})
set(CMAKE_OUTPUT_PATH ${CMAKE_BUILD_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_INSTALL_DIR ${CMAKE_SOURCE_DIR}/artifacts)

set(PROJECT_SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(PROJECT_SDK ${CMAKE_SOURCE_DIR}/sdk)
set(PROJECT_SDK_LIBS ${PROJECT_SDK}/libs)
set(PROJECT_SDK_BIN ${PROJECT_SDK}/bin)
set(PROJECT_VENDOR_DIR ${CMAKE_SOURCE_DIR}/support/vendor)
set(PROJECT_TARGETS_DIR ${CMAKE_SOURCE_DIR}/targets)

add_compile_definitions(
    "BUILD_NUMBER=\"${KESTREL_BUILD_NUMBER}\""
    "KESTREL_VERSION=\"${KESTREL_VERSION}\""
)

########################################################################################################################

if (NOT APPLE)
    add_subdirectory(sdk/vendor/LuaJIT)
endif()

add_subdirectory(sdk/vendor/imgui)
add_subdirectory(submodules/graphite)
add_subdirectory(sdk/bin/kdtool)
add_subdirectory(sdk/libs/libFoundation)
add_subdirectory(sdk/libs/libScripting)
add_subdirectory(sdk/libs/libDocumentation)
add_subdirectory(sdk/libs/libCodeGen)
add_subdirectory(sdk/libs/libInterpreter)
add_subdirectory(sdk/libs/libResource)
add_subdirectory(sdk/libs/libSound)
add_subdirectory(sdk/libs/libImage)
add_subdirectory(sdk/libs/libLexer)
add_subdirectory(sdk/libs/libKDL)
add_subdirectory(sdk/libs/libUI)
add_subdirectory(sdk/bin/kdl)
add_subdirectory(sdk/libs/libKestrel)
add_subdirectory(sdk/bin/player)

# Example/Test Projects
add_subdirectory(projects/GraphicsTestSuite)

# Extensions
#add_subdirectory(sdk/extra/tree-sitter/tree-sitter-kdl)

########################################################################################################################

enable_testing()
add_subdirectory(tests)
