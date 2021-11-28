//
// Created by Tom Hancocks on 21/11/2021.
//

#if !defined(AUDIO_CODEC_DESCRIPTOR_HPP)
#define AUDIO_CODEC_DESCRIPTOR_HPP

#include <cstdint>

namespace audio
{
    struct codec_descriptor
    {
    public:
        uint32_t sample_rate { 0 };
        uint32_t channels { 0 };
        uint32_t format_id { 0 };
        uint32_t format_flags { 0 };
        uint32_t bit_width { 0 };
        uint32_t bytes_per_frame { 0 };
        uint32_t frames_per_packet { 0 };
        uint32_t bytes_per_packet { 0 };
        uint32_t packet_count { 0 };

        codec_descriptor() = default;
    };
}

#endif //AUDIO_CODEC_DESCRIPTOR_HPP
