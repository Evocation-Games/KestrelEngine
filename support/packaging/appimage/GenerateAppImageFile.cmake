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

get_filename_component(SRC_DIR ${SRC} DIRECTORY)

if (NOT DEFINED KESTREL_NAME)
    set(KESTREL_NAME Kestrel)
    message(WARNING "Failed to determine KESTREL_NAME. Using default name \"${KESTREL_NAME}\"")
endif()

if (NOT DEFINED KESTREL_BIN_NAME)
    set(KESTREL_BIN_NAME Kestrel)
    message(WARNING "Failed to determine KESTREL_BIN_NAME. Using default name \"${KESTREL_BIN_NAME}\"")
endif()

if (NOT DEFINED KESTREL_ICON_NAME)
    set(KESTREL_ICON_NAME Kestrel)
    message(WARNING "Failed to determine KESTREL_ICON_NAME. Using default name \"${KESTREL_ICON_NAME}\"")
endif()

configure_file(${SRC} ${DST} @ONLY)