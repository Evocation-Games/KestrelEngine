echo "Building $3 Library"
cmake -DCMAKE_BUILD_TYPE=Release \
      -DFT_DISABLE_BROTLI=TRUE \
      -Hsubmodules/$1 \
      -Bcmake-$1-build-release
cmake --build cmake-$1-build-release -- -j2

mkdir -p vendor
cp cmake-$1-build-release/$2.a vendor/lib$1.a

rm -rf cmake-$1-build-release