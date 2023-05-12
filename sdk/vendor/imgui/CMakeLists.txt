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

########################################################################################################################
## libImGui
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