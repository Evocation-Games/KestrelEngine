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

#if !defined(PLAYER_ITEM_HPP)
#define PLAYER_ITEM_HPP

#include <memory>
#include <string>
#include "engine/core/audio/codec/audio_codec_descriptor.hpp"

namespace audio
{

    class player_item
    {
    public:
        explicit player_item(std::string file_path);
        explicit player_item(const codec_descriptor& codec);
        player_item(const codec_descriptor& codec, void *buffer, uint32_t buffer_size);

        player_item(const player_item& item) = default;
        player_item(player_item&& item) = default;

        ~player_item();

        auto allocate_buffer(uint32_t size = 0) -> void;
        auto internal_buffer_pointer() -> void *;

        [[nodiscard]] auto is_file_item() const -> bool;
        [[nodiscard]] auto file_path() const -> std::string;
        [[nodiscard]] auto sample_rate() const -> uint32_t;
        [[nodiscard]] auto buffer_size() const -> uint32_t;
        [[nodiscard]] auto bytes_per_packet() const -> uint32_t;
        [[nodiscard]] auto frames_per_packet() const -> uint32_t;
        [[nodiscard]] auto bytes_per_frame() const -> uint32_t;
        [[nodiscard]] auto channels() const -> uint8_t;
        [[nodiscard]] auto bit_width() const -> uint8_t;
        [[nodiscard]] auto format() const -> uint32_t;
        [[nodiscard]] auto format_flags() const -> uint32_t;

        auto set_sample_rate(uint32_t sample_rate) -> void;
        auto set_buffer_size(uint32_t buffer_size) -> void;
        auto set_bytes_per_packet(uint32_t bytes_per_packet) -> void;
        auto set_frames_per_packet(uint32_t frames_per_packet) -> void;
        auto set_bytes_per_frame(uint32_t bytes_per_frame) -> void;
        auto set_channels(uint8_t channels) -> void;
        auto set_bit_width(uint8_t bit_width) -> void;
        auto set_format(uint32_t format) -> void;
        auto set_format_flags(uint32_t format_flags) -> void;

    private:
        bool m_is_file { false };
        std::string m_file_path;
        void *m_buffer { nullptr };
        uint32_t m_sample_rate { 0 };
        uint32_t m_buffer_size { 0 };
        uint32_t m_bytes_per_packet { 0 };
        uint32_t m_frames_per_packet { 0 };
        uint32_t m_bytes_per_frame { 0 };
        uint8_t m_channels { 0 };
        uint8_t m_bit_width { 0 };
        uint32_t m_format_flags { 0 };
        uint32_t m_format_id { 0 };
    };

}

#endif //PLAYER_ITEM_HPP
