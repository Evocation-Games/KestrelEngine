cmake -H. -Bbuild
cmake --build build -- -j2
cmake --install build

cat build/libKestrel/lua_api_definition.cpp