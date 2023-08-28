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
project(Kestrel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

########################################################################################################################
## Preparations
set(KestrelAPIDefinition ${CMAKE_BUILD_DIR}/libKestrel/lua_api_definition.cpp)
file(MAKE_DIRECTORY ${CMAKE_BUILD_DIR}/libKestrel)

########################################################################################################################
## libKestrel
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

set(KESTREL_LIBS
    SIMD Data ResourceCore SoundCore QuickDraw ToolBox Image
    ImGUI Foundation Lexer UI
    ${OPENAL_LIBRARY} ${OPENGL_LIBRARIES}
    png bz2 z
    libluajit.a
)
set(KESTREL_INC_DIRS
    "${OPENGL_INCLUDE_DIRS}"
    "${PROJECT_SUBMODULE_DIR}/glm"
    "${PROJECT_SUBMODULE_DIR}/lua_bridge/Source"
    "${PROJECT_SUBMODULE_DIR}/graphite/libs"
    "${PROJECT_SUBMODULE_DIR}"
    ${PROJECT_SDK_LIBS}
)

if (APPLE)
    set(KESTREL_LIBS_DIR )
else()
    set(KESTREL_LIBS_DIR ${PROJECT_SUBMODULE_DIR}/LuaJIT/src)
endif()

if (APPLE)
    set(KESTREL_LIBS ${KESTREL_LIBS}
        "-framework Cocoa"
        "-framework MetalKit"
        "-framework Metal"
        "-framework CoreAudio"
        "-framework AudioUnit"
        "-framework AudioToolbox"
        "-framework CoreFoundation"
        "-framework AVFoundation"
        "-framework IOKit"
        "-framework GameController"
        "-framework Quartz"
        objc
        glfw
        freetype
    )
    set(KESTREL_INC_DIRS ${KESTREL_INC_DIRS}
        "${PROJECT_SUBMODULE_DIR}/freetype/include"
        "${PROJECT_SUBMODULE_DIR}/glfw/include"
    )
    file(GLOB_RECURSE KESTREL_SOURCES
        "${PROJECT_SDK_LIBS}/libKestrel/*.cpp"
        "${PROJECT_SDK_LIBS}/libKestrel/*.mm"
    )
elseif(LINUX)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Freetype REQUIRED)
    set(KESTREL_LIBS ${KESTREL_LIBS} ${GLEW_LIBRARIES} freetype glfw dl fontconfig pthread)
    set(KESTREL_INC_DIRS ${KESTREL_INC_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

    file(GLOB_RECURSE KESTREL_SOURCES
        "${PROJECT_SDK_LIBS}/libKestrel/*.cpp"
    )
elseif(WIN32)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Freetype REQUIRED)

    set(KESTREL_LIBS ${KESTREL_LIBS} GLEW::GLEW freetype glfw)
    set(KESTREL_INC_DIRS ${KESTREL_INC_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

    file(GLOB_RECURSE KESTREL_SOURCES
        "${PROJECT_SDK_LIBS}/libKestrel/*.cpp"
    )
endif()

add_library(Kestrel ${KESTREL_SOURCES} ${KestrelAPIDefinition})
target_include_directories(Kestrel PUBLIC ${KESTREL_INC_DIRS})
target_link_directories(Kestrel PUBLIC ${PROJECT_VENDOR_DIR} ${KESTREL_LIBS_DIR})
target_link_libraries(Kestrel ${KESTREL_LIBS})
add_dependencies(Kestrel kdtool)

########################################################################################################################
## libKestrel: Lua API Definition

if(WIN32)
    set(kdtool ${CMAKE_BUILD_DIR}/kdtool.exe)
else()
    set(kdtool ${CMAKE_BUILD_DIR}/kdtool)
endif()

file(GLOB_RECURSE KESTREL_LIB_HEADERS *.hpp)

add_custom_command(
    OUTPUT      ${KestrelAPIDefinition}
    COMMAND     ${kdtool}
                    -I ${PROJECT_SDK_LIBS}
                    -I ${CMAKE_SOURCE_DIR}/submodules
                    -I ${CMAKE_SOURCE_DIR}/submodules/graphite/libs
                    -I ${CMAKE_SOURCE_DIR}/submodules/lua_bridge/Source
                    -api ${KestrelAPIDefinition} -cxx -v
                    ${KESTREL_LIB_HEADERS}
    DEPENDS     ${KESTREL_LIB_HEADERS}
)

add_custom_target(KestrelAPIDefinition
    ALL
    DEPENDS     ${KestrelAPIDefinition}
    COMMENT     "Generating Lua API Definition for libKestrel"
)
add_dependencies(KestrelAPIDefinition kdtool)
add_dependencies(Kestrel KestrelAPIDefinition)

########################################################################################################################
## libKestrel: Generate Documentation

add_custom_target(KestrelLuaDocumentation
    ${kdtool}   -I ${PROJECT_SDK_LIBS}
                -I ${CMAKE_SOURCE_DIR}/submodules
                -I ${CMAKE_SOURCE_DIR}/submodules/graphite/libs
                -I ${CMAKE_SOURCE_DIR}/submodules/lua_bridge/Source
                -doctitle "Kestrel Lua API Documentation"
                -root https://tomhancocks.com/kestrel-api
                -documentation ${CMAKE_SOURCE_DIR}/docs/libKestrel -html
                ${KESTREL_LIB_HEADERS}
    DEPENDS     ${KESTREL_LIB_HEADERS}
    COMMENT     "Generating Lua API Documentation"
)
add_dependencies(KestrelLuaDocumentation kdtool)