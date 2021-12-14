
echo "Building Intel macOS $3 Library"
cmake -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=x86_64 \
  -Hsubmodules/$1 \
  -Bcmake-$1-intel-build-release
cmake --build cmake-$1-intel-build-release -- -j2

echo "Building Apple Silicon macOS $3 Library"
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -Hsubmodules/$1 \
  -Bcmake-$1-arm-build-release
cmake --build cmake-$1-arm-build-release -- -j2

echo "Merging $3 Libraries into single Universal Library"
mkdir -p vendor
lipo -create cmake-$1-intel-build-release/$2.a cmake-$1-arm-build-release/$2.a \
     -output vendor/lib$1.a

rm -rf cmake-$1-arm-build-release cmake-$1-intel-build-release