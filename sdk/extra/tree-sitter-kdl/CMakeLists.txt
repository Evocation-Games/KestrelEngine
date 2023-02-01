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

project(TreeSitter-KDL LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)

set(TREE_SITTER tree-sitter)

set(tree_sitter_lib tree-sitter-kdl)
set(tree_sitter_dir ${PROJECT_SDK}/extra/${tree_sitter_lib})
set(tree_sitter_src ${tree_sitter_dir}/src)

########################################################################################################################
## Generate TreeSitter KDL Grammar

add_custom_command(
    OUTPUT      ${tree_sitter_src}/parser.c
    COMMAND     ${TREE_SITTER} generate
    DEPENDS     ${tree_sitter_dir}/grammar.js
)

add_custom_target(GenerateTreeSitterKDLGrammar
    ALL
    DEPENDS     ${tree_sitter_src}/parser.c
    COMMENT     "Generating Tree Sitter grammar for KDL"
)

if (APPLE)
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rpath @loader_path/../Frameworks")
#    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

    # Build a Dynamic Library (dylib) on macOS...
    add_library(${tree_sitter_lib} SHARED ${tree_sitter_src}/parser.c)
    add_dependencies(${tree_sitter_lib} GenerateTreeSitterKDLGrammar)
    target_include_directories(${tree_sitter_lib} PUBLIC ${tree_sitter_src})

    target_link_options(${tree_sitter_lib} PUBLIC -F/Applications/Nova.app/Contents/Frameworks)
    target_link_libraries(${tree_sitter_lib} "-framework SyntaxKit")

else()
    # Build a static library...
    add_library(${tree_sitter_lib} STATIC ${tree_sitter_src}/parser.c)
    add_dependencies(${tree_sitter_lib} GenerateTreeSitterKDLGrammar)
    target_include_directories(${tree_sitter_lib} PUBLIC ${tree_sitter_src})

endif()

########################################################################################################################
## Nova (KDL Language Extension)


########################################################################################################################
## Nova (KDL Language Extension)
