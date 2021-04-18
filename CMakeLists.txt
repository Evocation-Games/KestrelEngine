# Copyright (c) 2020 Tom Hancocks
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
project(Kestrel VERSION 0.1.0 LANGUAGES CXX C)

set(KESTREL_NAME "Kestrel")

# CONFIGURATION ################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_SUBMODULES ${PROJECT_SOURCE_DIR}/submodules)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

# LUA ##########################################################################
add_library(Lua STATIC "${PROJECT_SUBMODULES}/lua/onelua.c")
target_compile_options(Lua PUBLIC "-DMAKE_LIB=1")
target_include_directories(Lua PUBLIC "${PROJECT_SUBMODULES}/lua")

# GRAPHITE #####################################################################
file(GLOB_RECURSE graphite_sources
	"${PROJECT_SUBMODULES}/graphite/libGraphite/*.cpp"
)
add_library(Graphite ${graphite_sources})
target_include_directories(Graphite PUBLIC "${PROJECT_SUBMODULES}/graphite")

# KDL ##########################################################################
file(GLOB_RECURSE kdl_sources
	"${PROJECT_SUBMODULES}/kdl/src/*.cpp"
)
add_executable(kdl ${kdl_sources})
target_include_directories(kdl PUBLIC
	"${PROJECT_SUBMODULES}/kdl/Submodules/Graphite"
	"${PROJECT_SUBMODULES}/kdl/src"
	"support/submodules/kdl"
)
target_link_libraries(kdl Graphite)

# GAME (BreakOut) #########################################################
#file(GLOB_RECURSE kestrel_game_files
#	"projects/KestrelDemo/*.kdl"
#	"projects/KestrelDemo/*.lua"
#)
#add_custom_target(KestrelGameCore ALL
#	DEPENDS kdl ${kestrel_game_files}
#)
#add_custom_command(TARGET KestrelGameCore
#	COMMAND ${CMAKE_BINARY_DIR}/kdl -o ${CMAKE_BINARY_DIR}/GameCore ${CMAKE_SOURCE_DIR}/projects/KestrelDemo/game.kdl
#	DEPENDS ${kestrel_game_files}
#)

# KESTREL ENGINE ###############################################################
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

if(LINUX)
	find_package(GLEW REQUIRED)
endif()

if (APPLE)
	file(GLOB_RECURSE kestrel_sources
		"engine/*.cpp"
		"engine/*.mm"
	)
else()
	file(GLOB_RECURSE kestrel_sources
		"engine/*.cpp"
	)
endif()

add_executable(Kestrel ${kestrel_sources})
target_include_directories(Kestrel PUBLIC
	"${OPENGL_INCLUDE_DIRS}"
	"${GLFW3_INCLUDE_DIRS}"
	"${FREETYPE_INCLUDE_DIRS}"
	"${PROJECT_SUBMODULES}/glm"
	"${PROJECT_SUBMODULES}/lua_bridge/Source"
	"${PROJECT_SUBMODULES}/graphite"
	"${PROJECT_SUBMODULES}"
	"engine"
)
target_link_libraries(Kestrel Graphite Lua ${OPENGL_LIBRARIES} glfw ${FREETYPE_LIBRARIES})
if(LINUX)
	target_include_directories(Kestrel PUBLIC "${GLEW_INCLUDE_DIRS}")
	target_link_libraries(Kestrel ${GLEW_LIBRARIES} "-lfontconfig")
	target_compile_options(Kestrel PUBLIC "-fpermissive")
elseif(APPLE)
	target_link_libraries(Kestrel "-framework Cocoa -framework MetalKit -framework Metal" objc)
endif()
add_dependencies(Kestrel kdl)

# MAC OS X APPLICATION BUNDLE ##################################################
if(APPLE)
	add_custom_command(TARGET Kestrel
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/support/macOS/Info.plist.in
			-D DST=${CMAKE_BINARY_DIR}/Info.plist
			-D KESTREL_NAME=${KESTREL_NAME}
			-P ${CMAKE_SOURCE_DIR}/support/macOS/GenerateInfoPlist.cmake
	)

	add_custom_command(TARGET Kestrel
		POST_BUILD
        COMMAND rm -rf "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app"
        COMMAND mkdir -p "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/MacOS"
        COMMAND mkdir -p "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/Resources"
        COMMAND xcrun -sdk macosx metal -I "${CMAKE_SOURCE_DIR}/engine" -c "${CMAKE_SOURCE_DIR}/engine/core/graphics/metal/shader.metal" -o "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/Resources/default.air"
        COMMAND xcrun -sdk macosx metallib "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/Resources/default.air" -o "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/Resources/default.metallib"
        COMMAND cp "${CMAKE_BINARY_DIR}/Kestrel" "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/MacOS/Kestrel"
        COMMAND cp "${CMAKE_BINARY_DIR}/GameCore.ndat" "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/Resources/GameCore.ndat"
        COMMAND cp "${CMAKE_BINARY_DIR}/Info.plist" "${CMAKE_BINARY_DIR}/${KESTREL_NAME}.app/Contents/Info.plist"
        DEPENDS KestrelScenario
	)
endif(APPLE)