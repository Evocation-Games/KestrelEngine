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
