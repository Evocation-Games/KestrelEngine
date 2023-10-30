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
project(OpenGLRenderer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

########################################################################################################################
## libOpenGLRenderer
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

set(OPENGLRENDERER_INCLUDE_DIRS
    ${PROJECT_SDK_LIBS}
    "${PROJECT_SUBMODULE_DIR}/glm"
)

set(OPENGLRENDERER_LIBS Data ${OPENGL_LIBRARIES})

if(APPLE)
    set(OPENGLRENDERER_INCLUDE_DIRS ${OPENGLRENDERER_INCLUDE_DIRS}
        "${PROJECT_SUBMODULE_DIR}/glfw/include"
    )

    set(OPENGLRENDERER_LIBS ${OPENGLRENDERER_LIBS}
         MacOSSupport glfw
    )

elseif(LINUX)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)

    set(OPENGLRENDERER_INCLUDE_DIRS ${OPENGLRENDERER_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS}
    )

    set(OPENGLRENDERER_LIBS ${OPENGLRENDERER_LIBS}
        ${GLEW_LIBRARIES} glfw
    )

elseif(WIN32)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)

    set(OPENGLRENDERER_INCLUDE_DIRS ${OPENGLRENDERER_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS}
    )

    set(OPENGLRENDERER_LIBS ${OPENGLRENDERER_LIBS} GLEW::GLEW glfw)

endif()

file(GLOB_RECURSE libOpenGLRenderer_Sources
    *.cpp
)

add_library(OpenGLRenderer ${libOpenGLRenderer_Sources})
target_link_libraries(OpenGLRenderer ${OPENGLRENDERER_LIBS})
target_include_directories(OpenGLRenderer PUBLIC ${OPENGLRENDERER_INCLUDE_DIRS})
target_link_directories(OpenGLRenderer PUBLIC ${PROJECT_VENDOR_DIR})
