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

#include <libSound/codecs/codec.hpp>

namespace sound::codec
{
    struct snd: public format
    {
        explicit snd(const std::string& path);
        explicit snd(const foundation::filesystem::path& path);
        explicit snd(const graphite::data::block& data);
        explicit snd(std::uint32_t sample_rate, std::uint8_t sample_bits, std::uint8_t channels, const graphite::data::block& samples);

        [[nodiscard]] auto byte_order() const -> graphite::data::byte_order override;

    private:
        auto decode(graphite::data::reader& reader) -> void override;
        auto encode(graphite::data::writer& writer) const -> void override;
    };
}