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

#include <libKestrel/sound/codec/ima4.hpp>

// MARK: - IMA4 LUTs

static constexpr std::int8_t ima4_index_table[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8
};

static constexpr std::int32_t ima4_step_table[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

// MARK: - Decoder

auto kestrel::sound::codec::ima4::decode(const descriptor& descriptor, graphite::data::reader& r) -> std::shared_ptr<player_item>
{
    // TODO: This is reallying on hard-coded constants and really shouldn't.
    // Determine the best way to calculate this values in the future.
    auto adjusted_descriptor = descriptor;
    adjusted_descriptor.bytes_per_packet = 34;
    adjusted_descriptor.frames_per_packet = 64;
    adjusted_descriptor.bytes_per_frame = 0;

    auto item = std::make_shared<player_item>(adjusted_descriptor);
    item->allocate_buffer((adjusted_descriptor.packet_count * (adjusted_descriptor.bytes_per_packet - 2)) << 2);

    // Prepare to read and parse the IMA4 data and decode it to LPCM 16
    auto out = reinterpret_cast<std::int16_t *>(item->internal_buffer_pointer());

    // Iterate through all the expected packets and decode them.
    for (std::uint32_t n = 0; n < adjusted_descriptor.packet_count; ++n) {
        auto preamble = r.read_short();
        auto packet = r.read_data(adjusted_descriptor.bytes_per_packet - 2);

        auto predictor = (std::int32_t)((std::int16_t)(preamble & 0xFF80));
        auto step_index = (std::int8_t)(preamble & 0x007F);
        auto step = ima4_step_table[step_index];
        std::uint8_t nibble = 0;
        std::int32_t diff = 0;

        for (std::uint32_t i = 0; i < packet.size(); ++i) {
            std::uint8_t data = packet.get<std::uint8_t>(i);

            nibble = data & 0xF;
            step_index = (std::int8_t)std::min(88, std::max(0, step_index + ima4_index_table[(unsigned)nibble]));
            std::uint8_t sign = nibble & 8;
            std::uint8_t delta = nibble & 7;
            diff = (std::int32_t)(step >> 3);
            if (delta & 4) diff += step;
            if (delta & 2) diff += (step >> 1);
            if (delta & 1) diff += (step >> 2);
            if (sign) predictor -= diff;
            else predictor += diff;
            predictor = std::min(INT16_MAX, std::max(INT16_MIN, predictor));
            *out++ = (std::int16_t)predictor;
            step = ima4_step_table[step_index];

            nibble = (data >> 4) & 0xF;
            step_index = (std::int8_t)std::min(88, std::max(0, step_index + ima4_index_table[(unsigned)nibble]));
            sign = nibble & 8;
            delta = nibble & 7;
            diff = (std::int32_t)(step >> 3);
            if (delta & 4) diff += step;
            if (delta & 2) diff += (step >> 1);
            if (delta & 1) diff += (step >> 2);
            if (sign) predictor -= diff;
            else predictor += diff;
            predictor = std::min(INT16_MAX, std::max(INT16_MIN, predictor));
            *out++ = (std::int16_t)predictor;
            step = ima4_step_table[step_index];

        }
    }

    item->set_bytes_per_packet(128);
    item->set_bit_width(16);
    item->set_frames_per_packet(item->channels());
    item->set_bytes_per_frame((item->bit_width() >> 3) * item->channels());
    item->set_format('lpcm');
    item->set_format_flags(0x4);

    return item;
}