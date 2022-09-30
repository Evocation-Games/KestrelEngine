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

if (APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
endif()

########################################################################################################################
## Kestrel Game Engine Project Configuration
project(KestrelGameEngine LANGUAGES CXX C)

set(KESTREL_NAME "Kestrel" CACHE STRING "Kestrel Package Name")
set(KESTREL_BIN_NAME "kestrel" CACHE STRING "Kestrel Binary Name")
set(KESTREL_ICON_NAME "kestrel_icon" CACHE STRING "Kestrel Packaged Icon Name")
set(KESTREL_ICON_PATH "support/packaging/icon-1024.png" CACHE STRING "Kestrel Icon Path")
set(KESTREL_PROJECT_PATH "projects/ExampleProject/example.kdlproj" CACHE STRING "Kestrel Project Path")
set(KESTREL_DATA_FILES_PATH "build/Scenario" CACHE STRING "Kestrel Data Files Path")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

if(CMAKE_BUILD_TYPE MATCHES ASan)
    message("Compiling with Address Sanitizer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
endif()

set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR})
set(PROJECT_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(PROJECT_SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(PROJECT_VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
set(CMAKE_OUTPUT_PATH ${CMAKE_BUILD_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

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
## ImGui
set(ImGUI_SOURCES
    "${PROJECT_SUBMODULE_DIR}/imgui/imgui.cpp"
    "${PROJECT_SUBMODULE_DIR}/imgui/imgui_draw.cpp"
    "${PROJECT_SUBMODULE_DIR}/imgui/imgui_tables.cpp"
    "${PROJECT_SUBMODULE_DIR}/imgui/imgui_widgets.cpp"
    "${PROJECT_SUBMODULE_DIR}/imgui/imgui_demo.cpp"
    "${PROJECT_SUBMODULE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${PROJECT_SUBMODULE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
    "${PROJECT_SUBMODULE_DIR}/imgui/misc/freetype/imgui_freetype.cpp"
    "${PROJECT_SUBMODULE_DIR}/ImGuiColorTextEdit/TextEditor.cpp"
)

if (APPLE)
    set(ImGUI_SOURCES ${ImGUI_SOURCES}
        "${PROJECT_SUBMODULE_DIR}/imgui/backends/imgui_impl_osx.mm"
        "${PROJECT_SUBMODULE_DIR}/imgui/backends/imgui_impl_metal.mm"
    )

    set(ImGUI_FREETYPE_INCLUDES "${PROJECT_SUBMODULE_DIR}/freetype/include")
else()
    find_package(Freetype REQUIRED)
    set(ImGUI_FREETYPE_INCLUDES ${FREETYPE_INCLUDE_DIRS})
endif()

add_library(ImGUI STATIC ${ImGUI_SOURCES})
target_compile_definitions(ImGUI PUBLIC -DIMGUI_ENABLE_FREETYPE)
target_include_directories(ImGUI PUBLIC
    "${PROJECT_SUBMODULE_DIR}/imgui"
    "${PROJECT_SUBMODULE_DIR}/ImGuiColorTextEdit"
    "${PROJECT_SUBMODULE_DIR}/glfw/include"
    ${ImGUI_FREETYPE_INCLUDES}
)

if (APPLE)
    set_property(TARGET ImGUI APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")
endif()

########################################################################################################################
## libKestrel
set(KESTREL_LIBS
    Graphite Lua
)
set(KESTREL_INC_DIRS
    "${PROJECT_SUBMODULE_DIR}/lua_bridge/Source"
    "${PROJECT_SUBMODULE_DIR}/graphite"
    "${PROJECT_SUBMODULE_DIR}"
    "libs"
)

file(GLOB_RECURSE KESTREL_SOURCES
    "libs/libKestrel/*.cpp"
)

add_library(Kestrel ${KESTREL_SOURCES})
target_include_directories(Kestrel PUBLIC ${KESTREL_INC_DIRS})
target_link_libraries(Kestrel ${KESTREL_LIBS})

########################################################################################################################
## Kestrel - Test Suite

if (EXISTS ${CMAKE_SOURCE_DIR}/KestrelTests.cmake)
    include(${PROJECT_SUBMODULE_DIR}/graphite/libTesting/testing.cmake)
    build_testing_library()
    add_testing_target(Kestrel ${CMAKE_SOURCE_DIR}/tests)
    include(${CMAKE_SOURCE_DIR}/KestrelTests.cmake)
endif()

########################################################################################################################
## Kestrel
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

set(KESTREL_BIN_LIBS
    Graphite Lua ImGUI
    ${OPENAL_LIBRARY} ${OPENGL_LIBRARIES}
    png bz2 z
)
set(KESTREL_BIN_INC_DIRS
    "${OPENGL_INCLUDE_DIRS}"
    "${PROJECT_SUBMODULE_DIR}/glm"
    "${PROJECT_SUBMODULE_DIR}/lua_bridge/Source"
    "${PROJECT_SUBMODULE_DIR}/graphite"
    "${PROJECT_SUBMODULE_DIR}"
    "engine"
)

if (APPLE)
    set(KESTREL_BIN_EXE_TYPE MACOSX_BUNDLE)
    file(GLOB_RECURSE KESTREL_BIN_SOURCES
        "engine/*.cpp"
        "engine/*.mm"
    )
    set(KESTREL_BIN_LIBS ${KESTREL_BIN_LIBS}
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
    set(KESTREL_BIN_INC_DIRS ${KESTREL_BIN_INC_DIRS}
        "${PROJECT_SUBMODULE_DIR}/freetype/include"
        "${PROJECT_SUBMODULE_DIR}/glfw/include"
    )

elseif(LINUX)
    set(KESTREL_BIN_EXE_TYPE )
    file(GLOB_RECURSE KESTREL_BIN_SOURCES
        "engine/*.cpp"
    )

    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Freetype REQUIRED)
    set(KESTREL_BIN_LIBS ${KESTREL_BIN_LIBS} ${GLEW_LIBRARIES} freetype glfw dl fontconfig pthread)
    set(KESTREL_BIN_INC_DIRS ${KESTREL_BIN_INC_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

elseif(WIN32)
    set(KESTREL_BIN_EXE_TYPE WIN32)
    file(GLOB_RECURSE KESTREL_BIN_SOURCES
        "engine/*.cpp"
    )

    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(Freetype REQUIRED)
    set(KESTREL_BIN_LIBS ${KESTREL_BIN_LIBS} GLEW::GLEW freetype glfw)
    set(KESTREL_BIN_INC_DIRS ${KESTREL_BIN_INC_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

endif()

add_executable(Kestrel-Bin ${KESTREL_BIN_EXE_TYPE} ${KESTREL_BIN_SOURCES})
target_include_directories(Kestrel-Bin PUBLIC ${KESTREL_BIN_INC_DIRS})
target_link_directories(Kestrel-Bin PUBLIC ${PROJECT_VENDOR_DIR})
target_link_libraries(Kestrel-Bin ${KESTREL_BIN_LIBS})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

if (APPLE OR WIN32)
    set_target_properties(Kestrel-Bin PROPERTIES OUTPUT_NAME ${KESTREL_NAME})
elseif(LINUX)
    set_target_properties(Kestrel-Bin PROPERTIES OUTPUT_NAME ${KESTREL_BIN_NAME})
endif()

########################################################################################################################
## Kestrel - Example Project
add_custom_target(Kestrel-GameProject
    ${CMAKE_BUILD_DIR}/kdl -f extended -o /kdl -f extended -o "${CMAKE_BUILD_DIR}/GameCore" "${CMAKE_SOURCE_DIR}/${KESTREL_PROJECT_PATH}"
)
add_dependencies(Kestrel-GameProject kdl-bin)
add_dependencies(Kestrel Kestrel-GameProject)

########################################################################################################################
## Kestrel - Icon
add_custom_target(generate-kestrel-icon
    cp "${CMAKE_SOURCE_DIR}/${KESTREL_ICON_PATH}" "${CMAKE_BUILD_DIR}/${KESTREL_ICON_NAME}.png"
)
add_dependencies(Kestrel-Bin generate-kestrel-icon)

########################################################################################################################
### Kestrel - Info Plist
if (APPLE)
    add_custom_target(generate-kestrel-info-plist
    	${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/support/packaging/macOS/Info.plist.in
    		             -D DST=${CMAKE_BUILD_DIR}/Info.plist
    		             -D KESTREL_NAME=${KESTREL_NAME}
                         -D KESTREL_BIN_NAME=${KESTREL_BIN_NAME}
                         -D KESTREL_ICON_NAME=${KESTREL_ICON_NAME}
    		             -P ${CMAKE_SOURCE_DIR}/support/packaging/macOS/GenerateInfoPlist.cmake
    )
    add_dependencies(Kestrel-Bin generate-kestrel-info-plist)
endif()

########################################################################################################################
## Kestrel - App Image Files
if (LINUX)
    set(APP_DIR ${PROJECT_INSTALL_DIR}/${KESTREL_NAME}.AppDir)
    set(APP_BIN_DIR ${APP_DIR}/usr/bin)
    set(APP_LIB_DIR ${APP_DIR}/usr/lib)
    set(APP_SHARE_DIR ${APP_DIR}/usr/share)

    add_custom_target(kestrel-app-image
        ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/support/packaging/appimage/kestrel.desktop.in
            -D DST=${CMAKE_BUILD_DIR}/${KESTREL_NAME}.desktop
            -D KESTREL_NAME=${KESTREL_NAME}
            -D KESTREL_BIN_NAME=${KESTREL_BIN_NAME}
            -D KESTREL_ICON_NAME=${KESTREL_ICON_NAME}
            -D APP_DIR=${APP_DIR}
            -D APP_LIB_DIR=${APP_LIB_DIR}
            -D APP_BIN_DIR=${APP_BIN_DIR}
            -P ${CMAKE_SOURCE_DIR}/support/packaging/appimage/GenerateAppImageFile.cmake

        COMMAND
        ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/support/packaging/appimage/kestrel.install.so.in
            -D DST=${CMAKE_BUILD_DIR}/install.so.sh
            -D KESTREL_NAME=${KESTREL_NAME}
            -D KESTREL_BIN_NAME=${KESTREL_BIN_NAME}
            -D KESTREL_ICON_NAME=${KESTREL_ICON_NAME}
            -D APP_DIR=${APP_DIR}
            -D APP_LIB_DIR=${APP_LIB_DIR}
            -D APP_BIN_DIR=${APP_BIN_DIR}
            -P ${CMAKE_SOURCE_DIR}/support/packaging/appimage/GenerateAppImageFile.cmake
    )
    add_dependencies(Kestrel-Bin kestrel-app-image)
endif()

########################################################################################################################
## Installation

install(TARGETS kdl-bin
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
        DESTINATION ${PROJECT_INSTALL_DIR})

if (APPLE)
    set_target_properties(Kestrel-Bin PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "${KESTREL_NAME}"
    )

    install(TARGETS Kestrel-Bin
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
            DESTINATION ${PROJECT_INSTALL_DIR})

    install(FILES ${CMAKE_BUILD_DIR}/GameCore.kdat
            DESTINATION ${PROJECT_INSTALL_DIR}/${KESTREL_NAME}.app/Contents/Resources)

    if (EXISTS ${KESTREL_DATA_FILES_PATH})
        install(DIRECTORY ${KESTREL_DATA_FILES_PATH}
                DESTINATION ${PROJECT_INSTALL_DIR}/${KESTREL_NAME}.app/Contents/Resources)
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

    install(TARGETS Kestrel-Bin
            PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            DESTINATION ${APP_BIN_DIR})

    install(FILES ${CMAKE_BUILD_DIR}/${KESTREL_NAME}.desktop
                  ${CMAKE_BUILD_DIR}/${KESTREL_ICON_NAME}.png
            DESTINATION ${APP_DIR})

    install(FILES ${CMAKE_BUILD_DIR}/AppRun
            DESTINATION ${APP_DIR}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)

    install(FILES ${CMAKE_BUILD_DIR}/GameCore.kdat
            DESTINATION ${APP_SHARE_DIR})

    if (EXISTS ${KESTREL_DATA_FILES_PATH})
        install(DIRECTORY ${KESTREL_DATA_FILES_PATH}
                DESTINATION ${APP_SHARE_DIR})
    endif()

    foreach(LIB ${KESTREL_RUNTIME_DEPENDENCIES})
        install(CODE "execute_process(COMMAND sh ${CMAKE_BUILD_DIR}/install.so.sh ${LIB})")
    endforeach()

    install(CODE "execute_process(COMMAND ${CMAKE_BUILD_DIR}/appimagetool ${APP_DIR} ${PROJECT_INSTALL_DIR}/${KESTREL_NAME})")

else()
    install(TARGETS Kestrel-Bin
            PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
            DESTINATION ${PROJECT_INSTALL_DIR})
endif()



