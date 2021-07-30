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

#include "core/asset/tga.hpp"
#include <iostream>
#include <utility>


// MARK: - Constructors

asset::tga::tga(const std::string& path)
{
    // TARGA images are stored in little endian, so set the byte order accordingly
    graphite::data::reader reader(path);
    reader.get()->set_byte_order(graphite::data::lsb);

    // TODO: Possibly handle any errors that occur?
    decode(reader);
}

asset::tga::tga(const std::shared_ptr<std::vector<char>>& data)
{
    auto ptr = std::make_shared<graphite::data::data>(data, data->size(), 0, graphite::data::lsb);
    graphite::data::reader reader(ptr, 0);
    decode(reader);
}

asset::tga::tga(std::shared_ptr<graphite::qd::surface>  surface)
    : m_surface(std::move(surface))
{

}

// MARK: - Decoding

auto asset::tga::decode(graphite::data::reader &reader) -> bool
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
    m_surface = std::make_shared<graphite::qd::surface>(header.width, header.height);

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

    // Start reading the image itself.
    int bytes_to_read = header.bits_per_pixel >> 3U;
    int n = 0;
    while (!reader.eof()) {
        switch (header.data_type_code) {
            // Uncompressed
            case 2: {
                auto v = reader.read_bytes(bytes_to_read);
                merge_bytes(n, v, 0, bytes_to_read);
                ++n;
                break;
            }
            // Compressed
            case 10: {
                auto v = reader.read_bytes(bytes_to_read + 1);
                int j = v[0] & 0x7F;
                merge_bytes(n, v, 1, bytes_to_read);
                ++n;
                if (v[0] & 0x80) { // RLE Chunk?
                    for (auto i = 0; i < j; ++i) {
                        merge_bytes(n, v, 1, bytes_to_read);
                        ++n;
                    }
                }
                else { // Normal Chunk?
                    for (auto i = 0; i < j; ++i) {
                        auto v2 = reader.read_bytes(bytes_to_read);
                        merge_bytes(n, v2, 0, bytes_to_read);
                        ++n;
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

auto asset::tga::merge_bytes(const int& position, const std::vector<char>& bytes, const int& offset, const int& size) -> void
{
    if (size == 4) {
        m_surface->set(position, graphite::qd::color(bytes[offset + 2], bytes[offset + 1], bytes[offset + 0], bytes[offset + 3]));
    }
    else if (size == 3) {
        m_surface->set(position, graphite::qd::color(bytes[offset + 2], bytes[offset + 1], bytes[offset + 0], 255));
    }
    else if (size == 2) {
        m_surface->set(position, graphite::qd::color((bytes[offset + 1] & 0x7c) << 1,
                                                     ((bytes[offset + 1] & 0x03) << 6) | ((bytes[offset + 0] & 0xe0) >> 2),
                                                     (bytes[offset + 0] & 0x1f) << 3,
                                                     bytes[offset + 1] & 0x80));
    }
}

// MARK: - Encoding

auto asset::tga::encode(graphite::data::writer &writer) -> void
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
    header.width = m_surface->size().width();
    header.height = m_surface->size().height();
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
    std::vector<graphite::qd::color> buffer;

    for (auto y = 0; y < header.height; ++y) {
        for (auto x = 0; x < header.width; ++x) {
            auto picker = m_surface->at(x, header.height - 1 - y);

            if (buffer.size() == 128 || (buffer.size() > 1 && buffer.back() == picker)) {
                auto n = buffer.size() - 1;
                if (buffer.back() == picker) {
                    n--;
                }

                writer.write_byte(n);
                for (auto i = 0; i < n; ++i) {
                    writer.write_byte(buffer[i].blue_component());
                    writer.write_byte(buffer[i].green_component());
                    writer.write_byte(buffer[i].red_component());
                    writer.write_byte(buffer[i].alpha_component());
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
                    writer.write_byte(buffer.back().blue_component());
                    writer.write_byte(buffer.back().green_component());
                    writer.write_byte(buffer.back().red_component());
                    writer.write_byte(buffer.back().alpha_component());
                    buffer.clear();
                    run = 0;
                }
            }
            else {
                if (run > 0) {
                    writer.write_byte(0x80 | (run - 1));
                    writer.write_byte(buffer.back().blue_component());
                    writer.write_byte(buffer.back().green_component());
                    writer.write_byte(buffer.back().red_component());
                    writer.write_byte(buffer.back().alpha_component());
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

auto asset::tga::surface() -> std::weak_ptr<graphite::qd::surface>
{
    return m_surface;
}

auto asset::tga::data() -> std::vector<char>
{
    auto data = std::make_shared<graphite::data::data>(graphite::data::lsb);
    graphite::data::writer writer(data);
    encode(writer);
    return *data->get();
}