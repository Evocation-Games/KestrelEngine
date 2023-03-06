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

#include <stdexcept>
#include <libImage/codecs/ppat/ppat.hpp>
#include <libGraphite/quickdraw/format/ppat.hpp>

// MARK: - Construction

image::codec::ppat::ppat(const std::string &path)
    : format(path)
{
    load_file_contents();
}

image::codec::ppat::ppat(const foundation::filesystem::path &path)
    : format(path)
{
    load_file_contents();
}

image::codec::ppat::ppat(const graphite::data::block &data)
    : format(data)
{
    load_data(data);
}

image::codec::ppat::ppat(graphite::quickdraw::surface &surface)
    : format(surface)
{}

// MARK: - Accessors

auto image::codec::ppat::byte_order() const -> graphite::data::byte_order
{
    return graphite::data::byte_order::msb;
}

// MARK: - Decoding

auto image::codec::ppat::decode(graphite::data::reader &reader) -> void
{
    graphite::quickdraw::ppat ppat(reader);
    m_surface = ppat.surface();
}

// MARK: - Encoding

auto image::codec::ppat::encode(graphite::data::writer &writer) const -> void
{
    graphite::quickdraw::ppat ppat(const_cast<graphite::quickdraw::surface&>(m_surface));
    ppat.encode(writer);
}