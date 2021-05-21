<p align="center">
  <img src="https://user-images.githubusercontent.com/681356/92330908-8530f800-f06a-11ea-842c-3ba3b6cc6ccd.png">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/version-v0.1_alpha-red.svg">
  <img src="https://img.shields.io/badge/license-MIT-blue.svg">
  <a href="https://discord.gg/u3dbBws"><img src="https://img.shields.io/discord/590385943425318912.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2"></a>
</p>

Welcome to the _Kestrel Game Engine_, a project that is centered around and focused on producing a cross-platform game engine for recreating and remastering sprite-based _Classic Macintosh_ games. Kestrel is being used by the upcoming _Cosmic Frontier: Override_ to deliver a remastered version of _Escape Velocity: Override_.

Kestrel is intended to be a fully moddable and extensible game engine that is focused on rebuilding and remastering Classic Macintosh games. In order to acheive this, rather than forcing all aspects of the game to be rebuilt and re-encoded in modern ways, Kestrel aims to emulate/reproduce some of the technologies that were employed by those old games such as Resource Files / Forks and QuickDraw.

## Building the Engine
To build the Kestrel engine you will need the following tools and dependencies installed on your system.

- Clang (macOS)
- GCC (Linux)
- MSYS2/MinGW (Windows)
- CMake
- OpenGL
- glfw3
- GLEW (Linux Only)
- FreeType 2

Other dependancies such as KDL, Graphite, Lua and LuaBridge are included as submodules and as such you do not need to worry about them.

Once you have all of the required dependancies installed, run the following two commands from the project root directory.

```sh
$ cmake -H. -Bbuild
$ cmake --build build -- -j2
```

This will work through all of the build tasks and place the result in the directory `./bin`. Depending on what your host OS is, will depend on what result you get. At current Kestrel has not been setup to use a cross compiler, and thus you will need to compile Kestrel on each of the platforms you wish to target separately.

After the build has finished you'll be left with the following artefacts in the `./bin` directory:

| macOS                  | Linux                | Windows |
|------------------------|----------------------|---------|
| Kestrel.app            | Kestrel (ELF Binary) |         |
| Kestrel (MachO Binary) | kdl                  |         |
| kdl                    | GameCore.ndat        |         |
| GameCore.ndat          |                      |         |
| Info.plist             |                      |         |

## Creating a new Project
If you wish to start creating a new game project using Kestrel, then run the following command from the Kestrel project root directory, replacing "Awesome Game" with the new of your new game project.

```sh
$ support/scripts/new-project.sh "Awesome Game"
```

This will create a blank game project in the directory `./projects/AwesomeGame/`.

## Building a Game
The process of building a game – not development of the game, just the build process – is done using KDL. If you have already followed the process for building the engine, then you'll already have KDL built in your `./bin` directory. Alternatively you can download copies/installers of KDL from the [KDL repository](https://github.com/tjhancocks/kdl.git).

For the remainder of this section we'll assume that your game is located at `./projects/AwesomeGame/` and that you have followed the recommended/standard project structure, and that you are using the KDL version that was built in the previous section.

To build and package the game data files, from the root directory of the project you can run the following command.

```sh
$ bin/kdl -o bin/GameCore projects/awesome-game/game.kdl
```

During this process, KDL will handle the majority of the heavy lifting with finding any depenancies and assembling all resources into the game data file.

The next steps depend on what platform you are targetting.

### macOS
On macOS the `GameCore.ndat` file is expected to be bundled inside the application bundle. You can copy it to the expected location by running the following command.

```sh
$ cp bin/GameCore.ndat bin/Kestrel.app/Contents/Resources/GameCore.ndat
```

Launching Kestrel.app will now load your game.

### Linux
On Linux the `GameCore.ndat` file is expected to be in the same directory as the main Kestrel executable, which in this case it already is.

### Additional Data Files
You may wish to split some of the additional game assets and resources in to seperate data files in order to better manage the project. Each of these files are expected to live in a `DataFiles` directory, located in the following positions:

On macOS the `DataFiles` directory is located at `Kestrel.app/Contents/Resources/DataFiles`.

On Linux the `DataFiles` directory is the same directory as the main Kestrel executable.

Kestrel will load the `GameCore.ndat` prior to loading the additional data files.


## Contributing
Contributions to Kestrel are welcome and appreciated. This is a big project, and will ultimately take the collective efforts of the community.

What type of contributions are being looked for? Anything on the list below is wanted.

- Proof reading and writing documentation.
- Code review.
- Submitting issues (bug fixes, feature requests, etc)
- Bug fixes & patches.
- Working on any issues/features.

For more details on how you might go about contributing to the project, please checkout the _Contributing_ document.

## Platform Completion / Features
The matrix below gives a very brief and high level overview of what aspects of the engine have been worked upon and what current state they are in. This does not give an intricate overview of the features supported by the engine.

| Feature | macOS                                                                         | Linux                                                                | Windows                                                                |
|---------|-------------------------------------------------------------------------------|----------------------------------------------------------------------|------------------------------------------------------------------------|
| Clang   | ![Clang](https://img.shields.io/badge/Clang-Supported-good.svg)               | ![Clang](https://img.shields.io/badge/Clang-Unknown-grey.svg)        | ![Clang](https://img.shields.io/badge/Clang-Not_Supported-black.svg)   |
| GCC     | ![GCC](https://img.shields.io/badge/GCC-Not_Supported-black.svg)              | ![GCC](https://img.shields.io/badge/GCC-Supported-good.svg)          | ![GCC](https://img.shields.io/badge/GCC-Supported_MSYS2/MinGW-good.svg)       |
| MSVC    | ![MSVC](https://img.shields.io/badge/MSVC-Not_Supported-black.svg)            | ![MSVC](https://img.shields.io/badge/MSVC-Not_Supported-black.svg)   | ![MSVC](https://img.shields.io/badge/MSVC-Planned-red.svg)     |
| OpenGL  | ![OpenGL](https://img.shields.io/badge/OpenGL-Supported-good.svg)             | ![OpenGL](https://img.shields.io/badge/OpenGL-Supported-good.svg)    | ![OpenGL](https://img.shields.io/badge/OpenGL-Supported-good.svg) |
| Metal   | ![Metal](https://img.shields.io/badge/Metal-Supported-good.svg)           | ![Metal](https://img.shields.io/badge/Metal-Not_Supported-black.svg) | ![Metal](https://img.shields.io/badge/Metal-Not_Supported-black.svg)   |
| Audio   | ![CoreAudio](https://img.shields.io/badge/Core_Audio-Not_Implemented-red.svg) | ![LibUnknown](https://img.shields.io/badge/Library_Unknown-grey.svg) | ![LibUnknown](https://img.shields.io/badge/Library_Unknown-grey.svg)   |

## License
The Kestrel Game Engine is distributed under the MIT License.
