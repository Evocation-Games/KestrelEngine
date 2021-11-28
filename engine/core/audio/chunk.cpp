//
// Created by Tom Hancocks on 25/11/2021.
//

#include <cstdlib>
#include "core/audio/chunk.hpp"

// MARK: - Destruction

audio::chunk::~chunk()
{
    if (data) {
        free(data);
    }
}

// MARK: - Codec

auto audio::chunk::apply(const audio::codec_descriptor &descriptor) -> void
{
    sample_rate = descriptor.sample_rate;
    format_id = descriptor.format_id;
    format_flags = descriptor.format_flags;
    bytes_per_packet = descriptor.bytes_per_packet;
    frames_per_packet = descriptor.frames_per_packet;
    bytes_per_frame = descriptor.bytes_per_frame;
    channels = descriptor.channels;
    bit_width = descriptor.bit_width;
    sample_rate = descriptor.sample_rate;
    packet_count = descriptor.packet_count;
}

// MARK: - Allocation

auto audio::chunk::allocate_space(uint32_t size) -> void
{
    if (data) {
        free(data);
        data = nullptr;
    }

    data_size = size;
    data = malloc(size);
}

// MARK: - Accessors

auto audio::chunk::internal_data_pointer() -> void *
{
    return data;
}
