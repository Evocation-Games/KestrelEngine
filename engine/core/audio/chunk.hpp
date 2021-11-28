//
// Created by Tom Hancocks on 21/11/2021.
//

#if !defined(AUDIO_CHUNK_HPP)
#define AUDIO_CHUNK_HPP

#include <cstdint>
#include "core/audio/audio_codec_descriptor.hpp"

namespace audio
{
    struct chunk
    {
        void *data { nullptr };
        uint32_t sample_rate { 0 };
        uint32_t data_size { 0 };
        uint32_t bytes_per_packet { 0 };
        uint32_t frames_per_packet { 0 };
        uint32_t bytes_per_frame { 0 };
        uint8_t channels { 0 };
        uint8_t bit_width { 0 };
        uint32_t format_flags { 0 };
        uint32_t format_id { 0 };
        uint32_t packet_count { 0 };

        chunk() = default;
        ~chunk();

        auto apply(const audio::codec_descriptor& descriptor) -> void;
        auto allocate_space(uint32_t size = 0) -> void;
        auto internal_data_pointer() -> void *;
    };
}

#endif //CHUNK_HPP
