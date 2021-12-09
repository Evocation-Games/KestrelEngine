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
