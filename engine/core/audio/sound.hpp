//
// Created by tomhancocks on 19/11/2021.
//

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
