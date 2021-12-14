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

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

########################################################################################################################
## Kestrel Game Engine Project Configuration
project(KestrelGameEngine LANGUAGES CXX C)

set(KESTREL_NAME "Kestrel")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR})
set(PROJECT_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(PROJECT_SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(PROJECT_VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

########################################################################################################################
## Lua
add_library(Lua STATIC ${PROJECT_SUBMODULE_DIR}/lua/onelua.c)
target_compile_definitions(Lua PUBLIC MAKE_LIB)
target_include_directories(Lua PUBLIC ${PROJECT_SUBMODULE_DIR}/lua)

########################################################################################################################
## Graphite
add_subdirectory(${PROJECT_SUBMODULE_DIR}/graphite)

########################################################################################################################
## KDL
add_subdirectory(${PROJECT_SUBMODULE_DIR}/kdl)

########################################################################################################################
## Kestrel
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

set(KESTREL_LIBS
    Graphite Lua
    ${OPENAL_LIBRARY} ${OPENGL_LIBRARIES}
    png bz2 z
)
set(KESTREL_INC_DIRS
    "${OPENGL_INCLUDE_DIRS}"
    "${PROJECT_SUBMODULE_DIR}/glm"
    "${PROJECT_SUBMODULE_DIR}/lua_bridge/Source"
    "${PROJECT_SUBMODULE_DIR}/graphite"
    "${PROJECT_SUBMODULE_DIR}"
    "engine"
)

if (APPLE)
    set(KESTREL_EXE_TYPE MACOSX_BUNDLE)
    file(GLOB_RECURSE KESTREL_SOURCES
        "engine/*.cpp"
        "engine/*.mm"
    )
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
        objc
        glfw
        freetype
    )
    set(KESTREL_INC_DIRS ${KESTREL_INC_DIRS}
        "${PROJECT_SUBMODULE_DIR}/freetype/include"
        "${PROJECT_SUBMODULE_DIR}/glfw/include"
    )

elseif(LINUX)
    set(KESTREL_EXE_TYPE )
    file(GLOB_RECURSE KESTREL_SOURCES
        "engine/*.cpp"
    )

    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Freetype REQUIRED)
    set(KESTREL_LIBS ${KESTREL_LIBS} ${GLEW_LIBRARIES} freetype glfw dl fontconfig pthread)
    set(KESTREL_INC_DIRS ${KESTREL_INC_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

elseif(WIN32)
    set(KESTREL_EXE_TYPE WIN32)
    file(GLOB_RECURSE KESTREL_SOURCES
        "engine/*.cpp"
    )

    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Freetype REQUIRED)
    set(KESTREL_LIBS ${KESTREL_LIBS} GLEW::GLEW freetype glfw)
    set(KESTREL_INC_DIRS ${KESTREL_INC_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

endif()

add_executable(Kestrel ${KESTREL_EXE_TYPE} ${KESTREL_SOURCES})
target_include_directories(Kestrel PUBLIC ${KESTREL_INC_DIRS})
target_link_directories(Kestrel PUBLIC ${PROJECT_VENDOR_DIR})
target_link_libraries(Kestrel ${KESTREL_LIBS})
add_dependencies(Kestrel kdl)

########################################################################################################################
## Kestrel - Info Plist
if (APPLE)
    add_custom_target(generate-kestrel-info-plist
    	${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/support/macOS/Info.plist.in
    		             -D DST=${CMAKE_BUILD_DIR}/Info.plist
    		             -D KESTREL_NAME=${KESTREL_NAME}
    		             -P ${CMAKE_SOURCE_DIR}/support/macOS/GenerateInfoPlist.cmake
    )
    add_dependencies(Kestrel generate-kestrel-info-plist)
endif()

########################################################################################################################
## Kestrel - Metal Dependencies
if (APPLE)
    add_custom_target(kestrel-metal-libs
        xcrun -sdk macosx metal -I "${CMAKE_SOURCE_DIR}/engine" -c "${CMAKE_SOURCE_DIR}/engine/core/graphics/metal/shader.metal" -o ${CMAKE_BUILD_DIR}/default.air
        COMMAND xcrun -sdk macosx metallib ${CMAKE_BUILD_DIR}/default.air -o ${CMAKE_BUILD_DIR}/default.metallib
    )
    add_dependencies(Kestrel kestrel-metal-libs)
endif()

########################################################################################################################
## Installation

install(TARGETS kdl
        DESTINATION ${PROJECT_INSTALL_DIR})

if (APPLE)
    install(TARGETS Kestrel
            DESTINATION ${PROJECT_INSTALL_DIR})

    install(FILES ${CMAKE_BUILD_DIR}/Info.plist
            DESTINATION ${PROJECT_INSTALL_DIR}/Kestrel.app/Contents)

    install(FILES ${CMAKE_BUILD_DIR}/default.metallib
                  ${CMAKE_BUILD_DIR}/default.air
            DESTINATION ${PROJECT_INSTALL_DIR}/Kestrel.app/Contents/Resources)
else()
    install(TARGETS Kestrel
            RUNTIME_DEPENDENCIES ${OPENAL_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw freetype
            DESTINATION ${PROJECT_INSTALL_DIR})
endif()
