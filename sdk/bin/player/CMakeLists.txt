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
## Project Configuration

project(KestrelPlayer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(PLAYER KestrelPlayer)
set(PLAYER_ICON_NAME icon.png)

if (APPLE)
    set(KESTREL_PLAYER_EXE_TYPE MACOSX_BUNDLE)
elseif(UNIX)
    set(KESTREL_PLAYER_EXE_TYPE )
    set(LINUX TRUE)
elseif(WIN32)
    set(KESTREL_PLAYER_EXE_TYPE WIN32)
endif()

########################################################################################################################
## Compilation
file(GLOB_RECURSE KestrelPlayer_Sources
    ${PROJECT_SDK_BIN}/player/src/*.cpp
)

add_executable(KestrelPlayer ${KESTREL_PLAYER_EXE_TYPE} ${KestrelPlayer_Sources})
target_include_directories(KestrelPlayer PUBLIC
    ${PROJECT_SDK_LIBS}
    ${PROJECT_SDK_BIN}/player
)
target_link_libraries(KestrelPlayer Kestrel)
add_dependencies(KestrelPlayer KDL-Tool)

if (LINUX)
    set_target_properties(KestrelPlayer PROPERTIES OUTPUT_NAME ${KESTREL_PLAYER_BIN_NAME})
else()
    set_target_properties(KestrelPlayer PROPERTIES OUTPUT_NAME ${KESTREL_PLAYER_NAME})
endif()

########################################################################################################################
## Player Icon
add_custom_command(
    TARGET KestrelPlayer POST_BUILD
    COMMAND cp "${PROJECT_SDK_BIN}/player/${PLAYER_ICON_NAME}" "${CMAKE_BUILD_DIR}/${PLAYER_ICON_NAME}"
    COMMENT "Creating Player Icon"
)

########################################################################################################################
## Player - info.plist (Apple Only)
if(APPLE)
    add_custom_command(
        TARGET KestrelPlayer POST_BUILD
        COMMAND
            ${CMAKE_COMMAND}
                -D SRC=${CMAKE_SOURCE_DIR}/support/packaging/macOS/Info.plist.in
                -D DST=${CMAKE_BUILD_DIR}/Info.plist
                -D KESTREL_NAME=${KESTREL_PLAYER_NAME}
                -D KESTREL_BIN_NAME=${KESTREL_PLAYER_BIN_NAME}
                -D KESTREL_ICON_NAME=${PLAYER_ICON_NAME}
                -P ${CMAKE_SOURCE_DIR}/support/packaging/macOS/GenerateInfoPlist.cmake
        COMMENT "Creating Player info.plist"
    )
endif()

########################################################################################################################
## Player - App Image (Linux Only)
if(LINUX)
    set(PLAYER_APP_DIR ${PROJECT_INSTALL_DIR}/${KESTREL_PLAYER_NAME}.AppDir)
    set(PLAYER_APP_BIN_DIR ${PLAYER_APP_DIR}/usr/bin)
    set(PLAYER_APP_LIB_DIR ${PLAYER_APP_DIR}/usr/lib)
    set(PLAYER_APP_SHARE_DIR ${PLAYER_APP_DIR}/usr/share)

    message("PLAYER_API_DIR = ${PLAYER_APP_DIR}")
    message("PLAYER_APP_BIN_DIR = ${PLAYER_APP_BIN_DIR}")
    message("PLAYER_APP_LIB_DIR = ${PLAYER_APP_LIB_DIR}")
    message("PLAYER_APP_SHARE_DIR = ${PLAYER_APP_SHARE_DIR}")

    add_custom_command(
        TARGET KestrelPlayer POST_BUILD
        COMMAND
            ${CMAKE_COMMAND}
                -D SRC=${CMAKE_SOURCE_DIR}/support/packaging/appimage/kestrel.desktop.in
                -D DST=${CMAKE_BUILD_DIR}/${KESTREL_PLAYER_NAME}.desktop
                -D KESTREL_NAME=${KESTREL_PLAYER_NAME}
                -D KESTREL_BIN_NAME=${KESTREL_PLAYER_BIN_NAME}
                -D KESTREL_ICON_NAME=${PLAYER_ICON_NAME}
                -D APP_DIR=${PLAYER_APP_DIR}
                -D APP_LIB_DIR=${PLAYER_APP_BIN_DIR}
                -D APP_BIN_DIR=${PLAYER_APP_BIN_DIR}
                -P ${CMAKE_SOURCE_DIR}/support/packaging/appimage/GenerateAppImageFile.cmake

        COMMAND
            ${CMAKE_COMMAND}
                -D SRC=${CMAKE_SOURCE_DIR}/support/packaging/appimage/kestrel.install.so.in
                -D DST=${CMAKE_BUILD_DIR}/install.so.sh
                -D KESTREL_NAME=${KESTREL_PLAYER_NAME}
                -D KESTREL_BIN_NAME=${KESTREL_PLAYER_BIN_NAME}
                -D KESTREL_ICON_NAME=${PLAYER_ICON_NAME}
                -D APP_DIR=${PLAYER_APP_DIR}
                -D APP_LIB_DIR=${PLAYER_APP_BIN_DIR}
                -D APP_BIN_DIR=${PLAYER_APP_BIN_DIR}
                -P ${CMAKE_SOURCE_DIR}/support/packaging/appimage/GenerateAppImageFile.cmake

        COMMENT "Creating App Image"
    )
endif()

########################################################################################################################
## Player - Basic Game Placeholder
if (WIN32)
    set(KDL ${CMAKE_BUILD_DIR}/kdl.exe)
else()
    set(KDL ${CMAKE_BUILD_DIR}/kdl)
endif()

add_custom_command(
    TARGET KestrelPlayer POST_BUILD
    COMMAND
        ${KDL} -f extended -o ${CMAKE_BUILD_DIR}/GameCore.kdat
        ${CMAKE_SOURCE_DIR}/projects/placeholder/placeholder.kdlproj
    COMMENT "Generating placeholder game for player"
)

########################################################################################################################
## Installation
if (APPLE)
    set_target_properties(KestrelPlayer PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "${KESTREL_PLAYER_NAME}")

    install(TARGETS KestrelPlayer
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
            DESTINATION ${PROJECT_INSTALL_DIR})

    install(FILES ${CMAKE_BUILD_DIR}/GameCore.kdat
            DESTINATION ${PROJECT_INSTALL_DIR}/${KESTREL_PLAYER_NAME}.app/Contents/Resources)

    if (EXISTS ${KESTREL_DATA_FILES_PATH})
        install(DIRECTORY ${KESTREL_DATA_FILES_PATH}
                DESTINATION ${PROJECT_INSTALL_DIR}/${KESTREL_PLAYER_NAME}.app/Contents/Resources)
    endif()

elseif(LINUX)
    set(KESTREL_RUNTIME_DEPENDENCIES
        ${OPENGL_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${GLEW_LIBRARY}
        ${GLFW3_LIBRARY}
        ${FREETYPE_LIBRARY}
    )

    set(CMAKE_INSTALL_PREFIX "/usr")

    install(TARGETS KestrelPlayer
            PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            DESTINATION ${PLAYER_APP_BIN_DIR})

    install(FILES ${PROJECT_INSTALL_DIR}/${KESTREL_PLAYER_NAME}.desktop
                  ${CMAKE_BUILD_DIR}/${KESTREL_ICON_NAME}
            DESTINATION ${PLAYER_APP_DIR})

    install(FILES ${CMAKE_BUILD_DIR}/AppRun
            DESTINATION ${PLAYER_APP_DIR}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)

    install(FILES ${CMAKE_BUILD_DIR}/GameCore.kdat
            DESTINATION ${PLAYER_APP_SHARE_DIR})

    if (EXISTS ${KESTREL_DATA_FILES_PATH})
        install(DIRECTORY ${KESTREL_DATA_FILES_PATH}
                DESTINATION ${PLAYER_APP_SHARE_DIR})
    endif()

    foreach(LIB ${KESTREL_RUNTIME_DEPENDENCIES})
        install(CODE "execute_process(COMMAND sh ${CMAKE_BUILD_DIR}/install.so.sh ${LIB})")
    endforeach()

    install(CODE "execute_process(COMMAND ${CMAKE_BUILD_DIR}/appimagetool ${PLAYER_APP_DIR} ${PROJECT_INSTALL_DIR}/${KESTREL_PLAYER_NAME})")

else()
    install(TARGETS KestrelPlayer
            PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            DESTINATION ${PROJECT_INSTALL_DIR})
endif()