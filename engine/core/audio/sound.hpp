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

#if !defined(KESTREL_SOUND_HPP)
#define KESTREL_SOUND_HPP

#include <cstdint>
#include <vector>

namespace audio
{
    class sound
    {
    public:
        struct packet
        {
            uint32_t sample_rate;
            uint8_t channels;
            uint8_t bits;
            void *bytes;
            uint32_t length;

            packet(uint32_t sample_rate, uint8_t channels, uint8_t bits, const void *bytes, uint32_t length);
            ~packet();
        };

    private:
        std::vector<packet> m_packets;
        uint32_t m_sample_rate { 0 };
        uint8_t m_sample_size { 0 };
        uint8_t m_channels { 0 };
        uint32_t m_playback_packet_idx { 0 };

    public:
        sound() = default;
        sound(sound&);
        sound(uint32_t sample_rate, uint8_t sample_size, uint8_t channels);
        auto add_packet(const void *bytes, uint32_t length) -> void;

        auto valid() const -> bool;

        auto reset_playback() -> void;
        auto playback_current_packet_idx() const -> uint32_t;
        auto playback_current_packet() const -> const packet&;
        auto playback_consume_packet() -> const packet&;
    };
}

#endif //KESTREL_SOUND_HPP
