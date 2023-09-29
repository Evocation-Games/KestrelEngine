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

#include <libKestrel/configuration.hpp>

kestrel::configuration::configuration(const int argc, const char **argv)
{
    // We need to parse the arguments being passed into the configuration.
    for (std::int32_t n = 1; n < argc; ++n) {
        std::string option(argv[n]);
        if (option == "--scale") {
            video.scale = std::strtod(argv[++n], nullptr);
        }
        else if (option == "--openal") {
            audio.desired_api = sound::api::openal;
        }
        else if (option == "--opengl") {
            renderer.desired_api = renderer::api::opengl;
        }
        else if (option == "--game") {
            data_files.core = argv[++n];
        }
        else if (option == "--support") {
            data_files.support = argv[++n];
        }
        else if (option == "--scenarios") {
            data_files.scenarios = argv[++n];
        }
        else if (option == "--mods") {
            data_files.mods = argv[++n];
        }
        else if (option == "--fonts") {
            data_files.fonts = argv[++n];
        }
        else {
            unparsed_options.emplace_back(option);
        }
    }
}