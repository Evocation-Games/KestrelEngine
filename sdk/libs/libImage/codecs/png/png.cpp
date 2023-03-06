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
#include <libImage/codecs/png/png.hpp>
#include <libImage/codecs/png/lodepng.hpp>

// MARK: - Construction

image::codec::png::png(const std::string &path)
    : format(path)
{
    load_file_contents();
}

image::codec::png::png(const foundation::filesystem::path &path)
    : format(path)
{
    load_file_contents();
}

image::codec::png::png(const graphite::data::block &data)
    : format(data)
{
    load_data(data);
}

image::codec::png::png(graphite::quickdraw::surface &surface)
    : format(surface)
{}

// MARK: - Accessors

auto image::codec::png::byte_order() const -> graphite::data::byte_order
{
    return graphite::data::byte_order::msb;
}

// MARK: - Decoding

auto image::codec::png::decode(graphite::data::reader &reader) -> void
{
    std::vector<std::uint8_t> image;
    std::uint32_t width, height;

    auto bytes = reader.read_bytes(reader.size());
    auto error = lodepng::decode(image, width, height, std::vector<std::uint8_t>(bytes.begin(), bytes.end()));
    if (error) {
        throw std::runtime_error("PNG Decoder: Decoding failed with error: " + std::to_string(error));
    }

    // Setup a quickdraw surface for the image to be read into. The buffer should be completely
    // black by default. This will be the default image in the event we fail to read.
    m_surface = graphite::quickdraw::surface(width, height);
    for (auto i = 0, offset = 0; i < width * height; ++i, offset += 4) {
        m_surface.set(i, graphite::quickdraw::rgb(
            image[offset], image[offset + 1], image[offset + 2], image[offset + 3]
        ));
    }
}

// MARK: - Encoding

auto image::codec::png::encode(graphite::data::writer &writer) const -> void
{
    auto size = m_surface.size();
    std::vector<std::uint8_t> png_data;
    std::vector<std::uint8_t> image_data(size.width * size.height * 4);

    for (auto y = 0; y < size.height; ++y) {
        for (auto x = 0; x < size.width; ++x) {
            image_data[size.width * y * 4 + x * 4 + 0] = m_surface.at(x, y).components.red;
            image_data[size.width * y * 4 + x * 4 + 1] = m_surface.at(x, y).components.green;
            image_data[size.width * y * 4 + x * 4 + 2] = m_surface.at(x, y).components.blue;
            image_data[size.width * y * 4 + x * 4 + 3] = m_surface.at(x, y).components.alpha;
        }
    }
    auto error = lodepng::encode(png_data, image_data, size.width, size.height);
    writer.write_bytes(png_data);

    if (error) {
       throw std::runtime_error("PNG Encoder: Encode failed with error: " + std::to_string(error));
    }
}