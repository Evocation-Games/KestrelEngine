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

echo "Building macOS Intel LuaJIT Library"
PROJECT_ROOT=`pwd`

export MACOSX_DEPLOYMENT_TARGET=10.14
cd submodules/LuaJIT \
  && make clean \
  && make HOST_CC="clang -arch x86_64" TARGET_CC="clang -arch x86_64" TARGET_STCC="clang -arch x86_64" TARGET_LD="clang -arch x86_64" \
  && make install DESTDIR=${PROJECT_ROOT}/build/luajit/x86_64

echo "Building macOS Apple Silicon LuaJIT Library"
make clean \
  && make HOST_CC="clang -arch arm64" TARGET_CC="clang -arch arm64" TARGET_STCC="clang -arch arm64" TARGET_LD="clang -arch arm64" \
  && make install DESTDIR=${PROJECT_ROOT}/build/luajit/arm64

echo "Merging LuaJIT Libraries into single Universal Library"
cd ${PROJECT_ROOT}
mkdir -p support/vendor
lipo -create ${PROJECT_ROOT}/build/luajit/x86_64/usr/local/lib/libluajit-5.1.a \
             ${PROJECT_ROOT}/build/luajit/arm64/usr/local/lib/libluajit-5.1.a \
     -output support/vendor/libluajit.a