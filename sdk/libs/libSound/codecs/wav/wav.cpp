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

#include <libSound/codecs/wav/wav.hpp>

// MARK: - Construction

sound::codec::wav::wav(const std::string &path)
    : format(path)
{
    load_file_contents();
}

sound::codec::wav::wav(const foundation::filesystem::path &path)
    : format(path)
{
    load_file_contents();
}

sound::codec::wav::wav(const graphite::data::block &data)
    : format(data)
{
    load_data(data);
}

sound::codec::wav::wav(std::uint32_t sample_rate, std::uint8_t sample_bits, std::uint8_t channels, const graphite::data::block& samples)
    : format(sample_rate, sample_bits, channels, samples)
{}

// MARK: - Accessors

auto sound::codec::wav::byte_order() const -> graphite::data::byte_order
{
    return graphite::data::byte_order::lsb;
}

// MARK: - Decoding

auto sound::codec::wav::find_chunk(graphite::data::reader &reader, uint64_t end_position, std::string chunk_id) -> bool
{
    while(reader.position() + 4 < end_position) {
        auto chunk_type = reader.read_cstr(4);
        if (chunk_type != chunk_id) {
            auto chunk_size = reader.read_long();
            reader.move(chunk_size);
        }
        else {
            return true;
        }
    }
    return false;
}

auto sound::codec::wav::decode(graphite::data::reader &reader) -> void
{
    if (reader.read_cstr(4) != "RIFF") {
        throw std::runtime_error("WAV Decoder: Expected 'RIFF' chunk");
    }

    auto end_position = reader.read_long() + reader.position();
    if (reader.read_cstr(4) != "WAVE") {
        throw std::runtime_error("WAV Decoder: Expected 'WAVE' format");
    }

    if (!find_chunk(reader, end_position, "fmt ")) {
        throw std::runtime_error("WAV Decoder: Expected 'fmt ' subchunk.");
    }

    fmt_subchunk_header fmt;
    fmt.size = reader.read_long();
    fmt.audio_format = reader.read_short();
    if (fmt.audio_format != 1 || fmt.size != 16) {
        throw std::runtime_error(
            "WAV Decoder: Expected PCM file (format 1, size 16; got format " + std::to_string(fmt.audio_format) + ", size "
            + std::to_string(fmt.size) + ")"
        );
    }

    fmt.num_channels = reader.read_short();
    fmt.sample_rate = reader.read_long();
    fmt.byte_rate = reader.read_long();
    fmt.block_align = reader.read_short();
    fmt.bits_per_sample = reader.read_short();

    if (!find_chunk(reader, end_position, "data")) {
        throw std::runtime_error("WAV Decoder: Expecte 'fmt ' subchunk");
    }

    auto data_size = reader.read_long();
    auto num_samples = data_size / fmt.block_align;
    auto bytes_per_sample = fmt.bits_per_sample / 8;
    auto sample_offset = fmt.bits_per_sample == 8 ? 0 : (1 << (fmt.bits_per_sample - 1));

    m_sample_bits = fmt.bits_per_sample;
    m_sample_rate = fmt.sample_rate;
    m_samples = graphite::data::block(fmt.num_channels * num_samples * 4);
    graphite::data::writer sample_writer(&m_samples);

    for (auto s = 0; s < num_samples; s++) {
        auto sample_start = reader.position();
        sample_writer.set_position(s * fmt.num_channels * 4);

        for (auto c = 0; c < fmt.num_channels; c++) {
            std::uint32_t sample = 0;
            for (auto b = 0; b < bytes_per_sample; b++) {
                sample |= reader.read_byte() << (b * 8);
            }
            if (bytes_per_sample > 1 && sample > sample_offset) {
                // Sign-extend for any value above 8 bits
                sample |= 0xFFFFFFFF << fmt.bits_per_sample;
            }
            // Add the sample offset to map the signed range onto the equivalent unsigned range
            sample = static_cast<std::int64_t>(sample) + sample_offset;
            sample_writer.write_long(sample);
        }

        // Move forward by the block align byte count -- may be different to the actual sample bytes
        reader.set_position(sample_start + fmt.block_align);
    }
}

// MARK: - Encoding

auto sound::codec::wav::encode(graphite::data::writer &writer) const -> void
{
    writer.change_byte_order(graphite::data::byte_order::lsb);

    auto sample_bytes = m_sample_bits / 8;
    auto num_samples = m_samples.size() / m_channels / sample_bytes;
    auto sample_offset = 1 << (m_sample_bits - 1);

    // Header
    writer.write_cstr("RIFF", 4);
    writer.write_long(36 + sample_bytes * m_channels * num_samples);
    writer.write_cstr("WAVE", 4);

    // FMT subchunk
    writer.write_cstr("fmt ", 4);
    writer.write_long(16); // SubChunk1Size
    writer.write_short(1); // PCM is format 1
    writer.write_short(m_channels);
    writer.write_long(m_sample_rate);
    writer.write_long(m_sample_rate * m_channels * sample_bytes); // byte rate
    writer.write_short(m_channels * sample_bytes); // block align
    writer.write_short(m_sample_bits);

    // data subchunk
    writer.write_cstr("data", 4);
    writer.write_long(sample_bytes * num_samples * m_channels);
    graphite::data::reader samples(&m_samples);

    for (auto s = 0; s < num_samples; ++s) {
        for (auto c = 0; c < m_channels; ++c) {
            samples.set_position(s * m_channels * 4);
            std::int32_t sample = static_cast<std::int64_t>(samples.read_long()) - sample_offset;

            if (m_sample_bits == 8) {
                // Byte range is 0 -> 255, midpoint 128
                writer.write_byte(static_cast<std::uint8_t>(sample));
            }
            else if (m_sample_bits == 16) {
                // Range -32768 -> 32767, midpoint 0
                writer.write_signed_short(sample);
            }
            else if (m_sample_bits == 24) {
                // Range -8338608 -> 8338607, midpoint 0
                writer.write_byte(sample & 0xFF);
                writer.write_byte((sample >> 8) & 0xFF);
                writer.write_byte((sample >> 16) & 0xFF);
            }
            else if (m_sample_bits == 32) {
                // Range -2147483648 -> 2147483647, midpoint 0
                writer.write_signed_long(sample);
            }
        }
    }
}