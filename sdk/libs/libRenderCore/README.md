# libRenderCore

This library is the core foundation of all rendering in the Kestrel Game 
Engine, responsible for constructing draw operations, abstracting and
managing resources and handling raw events across multiple different
graphics layer technologies.

Currently `libRenderCore` supports the following graphics layers:

- Metal (macOS only)
- OpenGL (Windows, Linux and macOS)

There is planned support for both _DirectX_ and _Vulkan_ in the future.
