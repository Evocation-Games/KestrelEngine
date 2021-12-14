echo "Building $3 Library"
cmake -DCMAKE_BUILD_TYPE=Release -Hsubmodules/$1 -Bcmake-$1-build-release
cmake --build cmake-$1-build-release -- -j2

mkdir -p vendor
cp cmake-$1-build-release/$2.* vendor/lib$1.*

rm -rf cmake-$1-build-release