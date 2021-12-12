FROM debian:bullseye-20210208-slim

RUN apt-get update -y -q && \
  apt-get install -y -q --no-install-recommends build-essential \
  cmake \
  clang++-10 \
  clang-10 \
  clang-tidy-10 \
  clang-tools-10 \
  libopenal-dev \
  libopengl-dev \
  libglfw3-dev \
  libglew-dev \
  libasound2-dev \
  libfreetype-dev \
  libfontconfig-dev && \
  apt-get clean

ENTRYPOINT ["/github/workspace/.github/actions/build_linux/entrypoint.sh"]
