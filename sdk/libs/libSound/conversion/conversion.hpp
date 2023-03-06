// Copyright (c) 2023 Tom Hancocks
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

#include <type_traits>
#include <libGraphite/data/data.hpp>

namespace sound::conversion
{
    struct format {
        static constexpr const char *snd = "SND";
        static constexpr const char *wav = "WAV";
    };

    struct sample_information {
        graphite::data::block samples;
        std::uint32_t rate;
        std::uint8_t bits;
        std::uint8_t channels;
    };

    auto samples(const std::string& source, const graphite::data::block& data) -> sample_information;
    auto data(const std::string& result, const sample_information& samples) -> graphite::data::block;
    auto perform(const std::string& source, const std::string& result, const graphite::data::block& data) -> graphite::data::block;
}
