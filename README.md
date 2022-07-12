<p align="center"><img src="https://raw.githubusercontent.com/EvocationGames/KestrelEngine/version-0.5/support/assets/KestrelLogoRepoHeader.png"></p>

<p align="center"><a href="https://github.com/EvocationGames/KestrelEngine/actions/workflows/build.yml"><img src="https://github.com/EvocationGames/KestrelEngine/actions/workflows/build.yml/badge.svg"></a> <img src="https://img.shields.io/badge/version-v0.7_beta-blue.svg">
<img src="https://img.shields.io/badge/license-MIT-blue.svg">
<a href="https://discord.gg/u3dbBws"><img src="https://img.shields.io/discord/590385943425318912.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2"></a>
</p>

---
# Kestrel Game Engine
Welcome to Kestrel! This project has the aim of developing a modern, cross platform game engine that can be used to recreate and remaster Classic Macintosh era games. It does this providing a number of APIs and facilities for reading and handling old formats from the classic era.

Kestrel is being used by the upcomming _Cosmic Frontier: Override_, a remastering of the classic _Escape Velocity: Override_.

Games in the Kestrel game engine are developed entirely within _Lua_, with all game assets and scripts being assembled into resource files. This can actually use either modernized formats, the old classic formats, or even a mix of the two. There will be more information about developing games for Kestrel in a future guide.

Currently Kestrel is reimplementing a number of technologies from the classic era, including; ResourceForks (mostly complete), QuickDraw (partial) and QuickTime (Sound Resource). Additional technologies will be introduced in the future and the existing ones will be fleshed out further.

Kestrel currently supports the following systems, though more are planned in the future.

- macOS 
	- Intel 64-bit (x86_64)
	- Apple Silicon (arm64)
- Linux 
	- Intel 64-bit (x86_64)
- Windows 
	- Intel 64-bit (x86_64)

## Why?
This question gets asked a lot. Why not just completely use modern technologies. Why bother to deal with old technologies at all?

Kestrel's early development has been for the most part guided by the requirements of Cosmic Frontier. One of the goals with the project is that it should retain compatibility with old plug-ins _Escape Velocity_ plug-ins. This necessitates being able to handle the old formats in some capacity. The other factor is the natural way in which resource files allow for plug-ins to effortless replace existing resources.

Of course it has added challenges, not least of all in the fact that the technologies are decades out of date, deprecated and/or gone. Hence why this project has reimplemented so many of them.

## Building Kestrel
It should be relatively straightforward to get started with building Kestrel for yourself. However if you are on Windows, then please refer to the _Building Kestrel on Windows_ section, as you will need to setup MinGW/MSYS2 first.

There are a few main requirements for building Kestrel; tooling, dependencies, etc. The first is `git` which we will need to clone the repository. Make sure you are in a location that you are happy to clone into

```sh
$ git clone --recurse-submodules https://github.com/EvocationGames/KestrelEngine.git
```

Once you have cloned the repository you can run the `setup` script to make sure you have everything installed and configured in your environment. To do this simply do

```sh
$ cd KestrelEngine
$ support/scripts/setup
```

The setup script will make sure that you have any tooling installed that is required to build the Kestrel engine, as well as explain the purpose of anything that needs your authentication to install.

Once the setup script has finished configuring your environment, and given you the go ahead to proceed, you can build the project. To build the project, all you need to do is:

```sh
$ support/scripts/build
$ support/scripts/install
```

There are two phases to this.

1. `build`: This does the bulk of the work. It prepares the build scripts and compiles all of the engine components. Everything is placed in the `./build` directory.
2. `install`: This will assemble the project into something that you can distribute to others, taking all of the components that were produced in the `build` phase. The results are placed in the `./bin` directory.

#### Configuring Kestrel Builds
It is likely that you will want to customise the Kestrel build. This will be common and indeed required for those that are building their own game with Kestrel.

There are a number of options that you can pass to the `build` script to customize the resulting build.

```sh
$ support/scripts/build --name="My Awesome Game" 
                        --bin-name="game" 
                        --project="path/to/kdlproj"
                        --icon="path/to/icon"
                        --build=Debug
                        --clean=on
```

It is generally recommended that you perform a clean of the build (`--clean=on`) if you are changing any of these options.

## Building Kestrel on Windows
In order to build Kestrel on Windows, you will need to use MinGW/MSYS2. Once you have that environment installed, you will be able to proceed with the steps above largely unhindered.

Once you have access to the MinGW/MSYS2 environment shell, you'll be able to return to the _Building Kestrel_ section as normal.

## Developing Games for Kestrel
_This section is to be completed_

## Contributing
Contributions to Kestrel are welcome and appreciated. This is a big project, and will ultimately take the collective efforts of the community.

What type of contributions are being looked for? Anything on the list below is wanted.

- Proof reading and writing documentation.
- Code review.
- Submitting issues (bug fixes, feature requests, etc)
- Bug fixes & patches.
- Working on any issues/features.

For more details on how you might go about contributing to the project, please checkout the Contributing document.

## License
Kestrel is distributed under the [MIT License]().
