// Copyright (c) 2021 Tom Hancocks
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

#include "core/audio/player/player_item.hpp"

// MARK: - Construction

audio::player_item::player_item(const codec_descriptor &codec)
    : m_sample_rate(codec.sample_rate),
      m_bytes_per_frame(codec.bytes_per_frame),
      m_frames_per_packet(codec.frames_per_packet),
      m_bytes_per_packet(codec.bytes_per_packet),
      m_channels(codec.channels),
      m_bit_width(codec.bit_width),
      m_format_id(codec.format_id),
      m_format_flags(codec.format_flags),
      m_buffer_size(codec.bytes_per_packet * codec.packet_count)
{

}

audio::player_item::player_item(const audio::codec_descriptor &codec, void *buffer, uint32_t buffer_size)
    : m_sample_rate(codec.sample_rate),
      m_bytes_per_frame(codec.bytes_per_frame),
      m_frames_per_packet(codec.frames_per_packet),
      m_bytes_per_packet(codec.bytes_per_packet),
      m_channels(codec.channels),
      m_bit_width(codec.bit_width),
      m_format_id(codec.format_id),
      m_format_flags(codec.format_flags),
      m_buffer(buffer),
      m_buffer_size(buffer_size)
{

}

// MARK: - Destruction

audio::player_item::~player_item()
{
    if (m_buffer) {
        free(m_buffer);
    }
}

// MARK: - Buffer Management

auto audio::player_item::allocate_buffer(uint32_t size) -> void
{
    auto buffer_size = (size == 0) ? m_buffer_size : size;
    if (m_buffer) {
        free(m_buffer);
        m_buffer = nullptr;
    }

    if ((m_buffer_size = buffer_size) == 0) {
        return;
    }

    m_buffer = malloc(buffer_size);
}

auto audio::player_item::internal_buffer_pointer() -> void *
{
    return m_buffer;
}

// MARK: - Accessors

auto audio::player_item::sample_rate() const -> uint32_t
{
    return m_sample_rate;
}

auto audio::player_item::buffer_size() const -> uint32_t
{
    return m_buffer_size;
}

auto audio::player_item::bytes_per_packet() const -> uint32_t
{
    return m_bytes_per_packet;
}

auto audio::player_item::frames_per_packet() const -> uint32_t
{
    return m_frames_per_packet;
}

auto audio::player_item::bytes_per_frame() const -> uint32_t
{
    return m_bytes_per_frame;
}

auto audio::player_item::channels() const -> uint8_t
{
    return m_channels;
}

auto audio::player_item::bit_width() const -> uint8_t
{
    return m_bit_width;
}

auto audio::player_item::format() const -> uint32_t
{
    return m_format_id;
}

auto audio::player_item::format_flags() const -> uint32_t
{
    return m_format_flags;
}

// MARK: - Setters

auto audio::player_item::set_sample_rate(uint32_t sample_rate) -> void
{
    m_sample_rate = sample_rate;
}

auto audio::player_item::set_buffer_size(uint32_t buffer_size) -> void
{
    m_buffer_size = buffer_size;
}

auto audio::player_item::set_bytes_per_packet(uint32_t bytes_per_packet) -> void
{
    m_bytes_per_packet = bytes_per_packet;
}

auto audio::player_item::set_frames_per_packet(uint32_t frames_per_packet) -> void
{
    m_frames_per_packet = frames_per_packet;
}

auto audio::player_item::set_bytes_per_frame(uint32_t bytes_per_frame) -> void
{
    m_bytes_per_frame = bytes_per_frame;
}

auto audio::player_item::set_channels(uint8_t channels) -> void
{
    m_channels = channels;
}

auto audio::player_item::set_bit_width(uint8_t bit_width) -> void
{
    m_bit_width = bit_width;
}

auto audio::player_item::set_format(uint32_t format) -> void
{
    m_format_id = format;
}

auto audio::player_item::set_format_flags(uint32_t format_flags) -> void
{
    m_format_flags = format_flags;
}

