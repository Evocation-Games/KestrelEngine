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

#include <utility>
#include <libKestrel/sound/player/player_item.hpp>

// MARK: - Construction

kestrel::sound::player_item::player_item(std::string file_path)
    : m_file_path(std::move(file_path)), m_is_file(true)
{

}

kestrel::sound::player_item::player_item(const codec::descriptor &codec)
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

kestrel::sound::player_item::player_item(const codec::descriptor &codec, void *buffer, std::uint32_t buffer_size)
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

kestrel::sound::player_item::~player_item()
{
    if (m_buffer) {
        free(m_buffer);
    }
}

// MARK: - Buffer Management

auto kestrel::sound::player_item::allocate_buffer(std::uint32_t size) -> void
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

auto kestrel::sound::player_item::internal_buffer_pointer() -> void *
{
    return m_buffer;
}

// MARK: - Accessors

auto kestrel::sound::player_item::is_file_item() const -> bool
{
    return m_is_file;
}

auto kestrel::sound::player_item::file_path() const -> std::string
{
    return m_file_path;
}

auto kestrel::sound::player_item::sample_rate() const -> std::uint32_t
{
    return m_sample_rate;
}

auto kestrel::sound::player_item::buffer_size() const -> std::uint32_t
{
    return m_buffer_size;
}

auto kestrel::sound::player_item::bytes_per_packet() const -> std::uint32_t
{
    return m_bytes_per_packet;
}

auto kestrel::sound::player_item::frames_per_packet() const -> std::uint32_t
{
    return m_frames_per_packet;
}

auto kestrel::sound::player_item::bytes_per_frame() const -> std::uint32_t
{
    return m_bytes_per_frame;
}

auto kestrel::sound::player_item::channels() const -> std::uint8_t
{
    return m_channels;
}

auto kestrel::sound::player_item::bit_width() const -> std::uint8_t
{
    return m_bit_width;
}

auto kestrel::sound::player_item::format() const -> std::uint32_t
{
    return m_format_id;
}

auto kestrel::sound::player_item::format_flags() const -> std::uint32_t
{
    return m_format_flags;
}

// MARK: - Setters

auto kestrel::sound::player_item::set_sample_rate(std::uint32_t sample_rate) -> void
{
    m_sample_rate = sample_rate;
}

auto kestrel::sound::player_item::set_buffer_size(std::uint32_t buffer_size) -> void
{
    m_buffer_size = buffer_size;
}

auto kestrel::sound::player_item::set_bytes_per_packet(std::uint32_t bytes_per_packet) -> void
{
    m_bytes_per_packet = bytes_per_packet;
}

auto kestrel::sound::player_item::set_frames_per_packet(std::uint32_t frames_per_packet) -> void
{
    m_frames_per_packet = frames_per_packet;
}

auto kestrel::sound::player_item::set_bytes_per_frame(std::uint32_t bytes_per_frame) -> void
{
    m_bytes_per_frame = bytes_per_frame;
}

auto kestrel::sound::player_item::set_channels(std::uint8_t channels) -> void
{
    m_channels = channels;
}

auto kestrel::sound::player_item::set_bit_width(std::uint8_t bit_width) -> void
{
    m_bit_width = bit_width;
}

auto kestrel::sound::player_item::set_format(std::uint32_t format) -> void
{
    m_format_id = format;
}

auto kestrel::sound::player_item::set_format_flags(std::uint32_t format_flags) -> void
{
    m_format_flags = format_flags;
}

