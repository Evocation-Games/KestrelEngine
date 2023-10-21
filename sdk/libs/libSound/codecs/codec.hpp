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

#include <libData/block.hpp>
#include <libData/writer.hpp>
#include <libData/reader.hpp>
#include <libFoundation/system/filesystem/path.hpp>

namespace sound::codec
{
    struct format
    {
        explicit format(const std::string& path) : m_path(path) {}
        explicit format(const foundation::filesystem::path& path) : m_path(path) {}
        explicit format(const data::block& data) {}

        explicit format(std::uint32_t sample_rate, std::uint8_t sample_bits, std::uint8_t channels, const data::block& samples)
            : m_sample_rate(sample_rate), m_sample_bits(sample_bits), m_channels(channels), m_samples(samples)
        {}

        [[nodiscard]] auto path() const -> foundation::filesystem::path
        {
            return m_path;
        }

        [[nodiscard]] auto data() const -> data::block
        {
            data::writer writer(byte_order());
            encode(writer);
            return std::move(*const_cast<data::block *>(writer.data()));
        }

        [[nodiscard]] virtual auto byte_order() const -> data::byte_order = 0;

        [[nodiscard]] auto sample_bits() const -> std::uint8_t
        {
            return m_sample_bits;
        }

        [[nodiscard]] auto sample_rate() const -> std::uint32_t
        {
            return m_sample_rate;
        }

        [[nodiscard]] auto channels() const -> std::uint8_t
        {
            return m_channels;
        }

        [[nodiscard]] auto samples() -> data::block
        {
            return m_samples;
        }

    protected:
        std::uint8_t m_sample_bits { 8 };
        std::uint8_t m_channels { 1 };
        std::uint32_t m_sample_rate { 1 };
        data::block m_samples;
        foundation::filesystem::path m_path;

        virtual auto decode(data::reader& reader) -> void = 0;
        virtual auto encode(data::writer& writer) const -> void = 0;

        auto load_file_contents() -> void
        {
            auto reader = data::reader::file(m_path.string());
            reader.change_byte_order(this->byte_order());
            decode(reader);
        }

        auto load_data(const data::block& data) -> void
        {
            data::reader reader(&data);
            reader.change_byte_order(this->byte_order());
            decode(reader);
        }
    };
}