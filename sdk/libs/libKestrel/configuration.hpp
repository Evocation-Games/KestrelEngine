// Copyright (c) 2022 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/util/availability.hpp>
#include <libKestrel/graphics/renderer/common/api.hpp>
#include <libKestrel/sound/api.hpp>

namespace kestrel
{
    struct configuration
    {
        configuration() = default;
        configuration(int argc, const char *argv[]);

        std::vector<std::string> unparsed_options;

        struct {
            std::string string;
        } version;

        struct {
            std::uint16_t width { 1280 };
            std::uint16_t height { 800 };
            double scale { 0.0 };
            bool fullscreen { true };
            std::string title { "Kestrel" };
        } video;

        struct {
            enum renderer::api desired_api {
#if TARGET_MACOS
                renderer::api::metal
#else
                renderer::api::opengl
#endif
            };
        } renderer;

        struct {
            enum sound::api desired_api {
#if TARGET_MACOS
                sound::api::core_audio
#else
                sound::api::openal
#endif
            };
        } audio;

        struct {
            std::string core;
            std::string support;
            std::string scenarios;
            std::string mods;
            std::string fonts;
            std::vector<std::string> recognized_extensions { "rsrc", "rsrx" };
            bool include_dot_files { false };
        } data_files;
    };
}