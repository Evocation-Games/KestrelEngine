// Copyright (c) 2020 Tom Hancocks
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

#include <iostream>
#include <utility>
#include <stdexcept>
#include <libKestrel/graphics/image/tga.hpp>

// MARK: - Constructors

kestrel::image::tga::tga(const std::string& path)
{
    // TARGA images are stored in little endian, so set the byte order accordingly
    graphite::data::block source(path);
    graphite::data::reader reader(&source);
    reader.change_byte_order(graphite::data::byte_order::lsb);

    // TODO: Possibly handle any errors that occur?
    decode(reader);
}

kestrel::image::tga::tga(const graphite::data::block& data)
{
    graphite::data::reader tga_reader(&data);
    tga_reader.change_byte_order(graphite::data::byte_order::lsb);
    decode(tga_reader);
}

kestrel::image::tga::tga(graphite::quickdraw::surface& surface)
    : m_surface(std::move(surface))
{

}

// MARK: - Decoding

auto kestrel::image::tga::decode(graphite::data::reader &reader) -> bool
{
    // Read the TGA header from the image
    tga::header header{};
    header.id_length = reader.read_byte();
    header.color_map_type = reader.read_byte();
    header.data_type_code = reader.read_byte();
    header.color_map_origin = reader.read_short();
    header.color_map_length = reader.read_short();
    header.color_map_depth = reader.read_byte();
    header.x_origin = reader.read_short();
    header.y_origin = reader.read_short();
    header.width = reader.read_short();
    header.height = reader.read_short();
    header.bits_per_pixel = reader.read_byte();
    header.image_descriptor = reader.read_byte();

    // Setup a QuickDraw surface for the image to be read into. The buffer should be completely
    // black by default. This will be the "default" image in the event we fail to read.
    m_surface = graphite::quickdraw::surface(header.width, header.height);

    // Make sure this is a TGA image that we can handle.
    if (header.data_type_code != 2 && header.data_type_code != 10) {
        std::cerr << "TGA Decoder: Can only handle image type 2 and 10" << std::endl;
        return false;
    }

    if (header.bits_per_pixel != 16 && header.bits_per_pixel != 24 && header.bits_per_pixel != 32) {
        std::cerr << "TGA Decoder: Can only handle image depths of 16, 24 or 32-bits" << std::endl;
        return false;
    }

    if (header.color_map_type != 0 && header.color_map_type != 1) {
        std::cerr << "TGA Decoder: Can only handle color map types of 0 and 1" << std::endl;
        return false;
    }

    // Ignore unrequired information.
    reader.move(header.id_length);
    reader.move(header.color_map_type * header.color_map_length);

    int start = (header.height - 1) * header.width;

    // Start reading the image itself.
    int bytes_to_read = header.bits_per_pixel >> 3U;
    int n = 0;
    while (!reader.eof()) {
        switch (header.data_type_code) {
            // Uncompressed
            case 2: {
                auto v = reader.read_data(bytes_to_read);
                merge_bytes(start + n, v, 0, bytes_to_read);
                ++n;
                if (n >= header.width) {
                    start -= header.width;
                    n = 0;
                }
                break;
            }
            // Compressed
            case 10: {
                auto v = reader.read_data(bytes_to_read + 1);
                int j = v.get<uint8_t>(0) & 0x7F;
                merge_bytes(start + n, v, 1, bytes_to_read);
                ++n;
                if (n >= header.width) {
                    start -= header.width;
                    n = 0;
                }

                if (v.get<uint8_t>(0) & 0x80) { // RLE Chunk?
                    for (auto i = 0; i < j; ++i) {
                        merge_bytes(start + n, v, 1, bytes_to_read);
                        ++n;
                        if (n >= header.width) {
                            start -= header.width;
                            n = 0;
                        }
                    }
                }
                else { // Normal Chunk?
                    for (auto i = 0; i < j; ++i) {
                        auto v2 = reader.read_data(bytes_to_read);
                        merge_bytes(start + n, v2, 0, bytes_to_read);
                        ++n;
                        if (n >= header.width) {
                            start -= header.width;
                            n = 0;
                        }
                    }
                }
                break;
            }
            default: {
                throw std::logic_error("Illegal data_type_code encountered: " + std::to_string(header.data_type_code));
            }
        }
    }

    // Finished
    return true;
}

auto kestrel::image::tga::merge_bytes(int position, const graphite::data::block& bytes, int offset, int size) -> void
{
    if (size == 4) {
        auto r = bytes.get<uint8_t>(offset + 2);
        auto g = bytes.get<uint8_t>(offset + 1);
        auto b = bytes.get<uint8_t>(offset + 0);
        auto a = bytes.get<uint8_t>(offset + 3);
        m_surface.set(position, graphite::quickdraw::rgb(r, g, b, a));
    }
    else if (size == 3) {
        auto r = bytes.get<uint8_t>(offset + 2);
        auto g = bytes.get<uint8_t>(offset + 1);
        auto b = bytes.get<uint8_t>(offset + 0);
        m_surface.set(position, graphite::quickdraw::rgb(r, g, b, 255));
    }
    else if (size == 2) {
        auto f = bytes.get<uint8_t>(offset + 1);
        auto s = bytes.get<uint8_t>(offset + 0);
        m_surface.set(position, graphite::quickdraw::rgb((s & 0x7c) << 1,
                                                         ((s & 0x03) << 6) | ((f & 0xe0) >> 2),
                                                         (f & 0x1f) << 3,
                                                         s & 0x80));
    }
}

// MARK: - Encoding

auto kestrel::image::tga::encode(graphite::data::writer &writer) -> void
{
    // Formulate a TGA header
    tga::header header{};
    header.id_length = 0;
    header.color_map_type = 0;
    header.data_type_code = 10;
    header.color_map_origin = 0;
    header.color_map_length = 0;
    header.color_map_depth = 0;
    header.x_origin = 0;
    header.y_origin = 0;
    header.width = m_surface.size().width;
    header.height = m_surface.size().height;
    header.bits_per_pixel = 32;
    header.image_descriptor = 0;

    writer.write_byte(header.id_length);
    writer.write_byte(header.color_map_type);
    writer.write_byte(header.data_type_code);
    writer.write_short(header.color_map_origin);
    writer.write_short(header.color_map_length);
    writer.write_byte(header.color_map_depth);
    writer.write_short(header.x_origin);
    writer.write_short(header.y_origin);
    writer.write_short(header.width);
    writer.write_short(header.height);
    writer.write_byte(header.bits_per_pixel);
    writer.write_byte(header.image_descriptor);

    // Start compressing and writing the image data.
    int run = 0;
    bool dirty = false;
    std::vector<graphite::quickdraw::color> buffer;

    for (auto y = 0; y < header.height; ++y) {
        for (auto x = 0; x < header.width; ++x) {
            auto picker = m_surface.at(x, header.height - 1 - y);

            if (buffer.size() == 128 || (buffer.size() > 1 && buffer.back() == picker)) {
                auto n = buffer.size() - 1;
                if (buffer.back() == picker) {
                    n--;
                }

                writer.write_byte(n);
                for (auto i = 0; i < n; ++i) {
                    writer.write_byte(buffer[i].components.blue);
                    writer.write_byte(buffer[i].components.green);
                    writer.write_byte(buffer[i].components.red);
                    writer.write_byte(buffer[i].components.alpha);
                }

                if (buffer.back() == picker) {
                    buffer.clear();
                    buffer.emplace_back(picker);
                    run = 2;
                }
            }
            else if (!buffer.empty() && buffer.back() == picker) {
                ++run;

                if (run == 128) {
                    writer.write_byte(0x80 | (run - 1));
                    writer.write_byte(buffer.back().components.blue);
                    writer.write_byte(buffer.back().components.green);
                    writer.write_byte(buffer.back().components.red);
                    writer.write_byte(buffer.back().components.alpha);
                    buffer.clear();
                    run = 0;
                }
            }
            else {
                if (run > 0) {
                    writer.write_byte(0x80 | (run - 1));
                    writer.write_byte(buffer.back().components.blue);
                    writer.write_byte(buffer.back().components.green);
                    writer.write_byte(buffer.back().components.red);
                    writer.write_byte(buffer.back().components.alpha);
                    buffer.clear();
                    run = 0;
                }
                buffer.emplace_back(picker);
                run = 1;
            }

        }
    }
}

// MARK: - Accessors

auto kestrel::image::tga::surface() const -> const graphite::quickdraw::surface&
{
    return m_surface;
}

auto kestrel::image::tga::data() -> std::vector<char>
{
    graphite::data::writer writer;
    writer.change_byte_order(graphite::data::byte_order::lsb);
    encode(writer);

    std::vector<char> v;
    for (auto i = 0; i < writer.data()->size(); ++i) {
        v.emplace_back(writer.data()->get<char>(i));
    }
    return v;
}