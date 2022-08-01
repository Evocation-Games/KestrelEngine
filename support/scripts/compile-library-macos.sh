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

echo "Building Intel macOS $3 Library"
cmake -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=x86_64 \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 \
  -DFT_DISABLE_BROTLI=TRUE \
  -Hsubmodules/$1 \
  -Bcmake-$1-intel-build-release
cmake --build cmake-$1-intel-build-release -- -j2

echo "Building Apple Silicon macOS $3 Library"
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 \
  -DFT_DISABLE_BROTLI=TRUE \
  -Hsubmodules/$1 \
  -Bcmake-$1-arm-build-release
cmake --build cmake-$1-arm-build-release -- -j2

echo "Merging $3 Libraries into single Universal Library"
mkdir -p vendor
lipo -create cmake-$1-intel-build-release/$2.a cmake-$1-arm-build-release/$2.a \
     -output vendor/lib$1.a

rm -rf cmake-$1-arm-build-release cmake-$1-intel-build-release