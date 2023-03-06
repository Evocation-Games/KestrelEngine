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

#include <libFoundation/system/filesystem/path.hpp>
#include <libGraphite/data/data.hpp>
#include <libGraphite/data/writer.hpp>
#include <libGraphite/data/reader.hpp>
#include <libGraphite/quickdraw/support/surface.hpp>

namespace image::codec
{
    struct format
    {
        explicit format(const std::string& path) : m_path(path) {}
        explicit format(const foundation::filesystem::path& path) : m_path(path) {}
        explicit format(const graphite::data::block& data) {}
        explicit format(graphite::quickdraw::surface& surface) : m_surface(surface) {}

        [[nodiscard]] auto path() const -> foundation::filesystem::path
        {
            return m_path;
        }

        [[nodiscard]] auto data() const -> graphite::data::block
        {
            graphite::data::writer writer(byte_order());
            encode(writer);
            return std::move(*const_cast<graphite::data::block *>(writer.data()));
        }

        [[nodiscard]] virtual auto byte_order() const -> graphite::data::byte_order = 0;
        auto surface() -> graphite::quickdraw::surface& { return m_surface; }

    protected:
        foundation::filesystem::path m_path;
        graphite::quickdraw::surface m_surface;

        virtual auto decode(graphite::data::reader& reader) -> void = 0;
        virtual auto encode(graphite::data::writer& writer) const -> void = 0;

        auto load_file_contents() -> void
        {
            auto reader = graphite::data::reader::file(m_path.string());
            reader.change_byte_order(this->byte_order());
            decode(reader);
        }

        auto load_data(const graphite::data::block& data) -> void
        {
            graphite::data::reader reader(&data);
            reader.change_byte_order(this->byte_order());
            decode(reader);
        }

    };
}