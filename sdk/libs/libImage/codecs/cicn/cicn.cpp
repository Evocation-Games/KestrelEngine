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
#include <libImage/codecs/cicn/cicn.hpp>
#include <libGraphite/quickdraw/format/cicn.hpp>

// MARK: - Construction

image::codec::cicn::cicn(const std::string &path)
    : format(path)
{
    load_file_contents();
}

image::codec::cicn::cicn(const foundation::filesystem::path &path)
    : format(path)
{
    load_file_contents();
}

image::codec::cicn::cicn(const graphite::data::block &data)
    : format(data)
{
    load_data(data);
}

image::codec::cicn::cicn(graphite::quickdraw::surface &surface)
    : format(surface)
{}

// MARK: - Accessors

auto image::codec::cicn::byte_order() const -> graphite::data::byte_order
{
    return graphite::data::byte_order::msb;
}

// MARK: - Decoding

auto image::codec::cicn::decode(graphite::data::reader &reader) -> void
{
    graphite::quickdraw::cicn cicn(reader);
    m_surface = cicn.surface();
}

// MARK: - Encoding

auto image::codec::cicn::encode(graphite::data::writer &writer) const -> void
{
    graphite::quickdraw::cicn cicn(const_cast<graphite::quickdraw::surface&>(m_surface));
    cicn.encode(writer);
}