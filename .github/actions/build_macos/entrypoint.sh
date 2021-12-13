pacman -s cmake libopenal-dev libopengl-dev libglfw3-dev libglew libfreetype-dev

cmake -H. -Bbuild
cmake --build build -- -j2