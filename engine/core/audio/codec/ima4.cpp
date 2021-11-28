//
// Created by Tom Hancocks on 21/11/2021.
//

#include "core/audio/codec/ima4.hpp"

// MARK: - IMA4 LUTs

static constexpr int32_t ima4_index_table[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8
};

static constexpr int32_t ima4_step_table[89] = {
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

auto audio::ima4::decode(const audio::codec_descriptor& descriptor, graphite::data::reader& r) -> std::shared_ptr<audio::chunk>
{
    // TODO: This is reallying on hard-coded constants and really shouldn't.
    // Determine the best way to calculate this values in the future.
    auto chunk = std::make_shared<audio::chunk>();
    chunk->apply(descriptor);
    chunk->bytes_per_packet = 34;
    chunk->frames_per_packet = 64;
    chunk->bytes_per_frame = 0;

    // Prepare to read and parse the IMA4 data and decode it to LPCM 16
    chunk->allocate_space((descriptor.packet_count * (chunk->bytes_per_packet - 2)) << 2);
    auto out = reinterpret_cast<int16_t *>(chunk->internal_data_pointer());

    // Iterate through all the expected packets and decode them.
    for (uint32_t n = 0; n < descriptor.packet_count; ++n) {
        auto preamble = r.read_short();
        auto packet = r.read_data(chunk->bytes_per_packet - 2);

        int32_t predictor = preamble & 0xFF80;
        int32_t step_index = preamble & 0x007F;
        int16_t step = ima4_step_table[step_index];

        for (uint32_t i = 0; i < descriptor.bytes_per_packet - 2; ++i) {
            uint8_t data = packet->at(i);
            uint8_t lower_nibble = data & 0x0F;
            uint8_t upper_nibble = (data & 0xF0) >> 4;

            // decode the lower nibble
            step_index += ima4_index_table[lower_nibble];
            int16_t sign = lower_nibble & 0x8;
            int16_t delta = lower_nibble & 0x7;
            int16_t diff = step >> 8;
            if (delta & 0x4) diff += step;
            if (delta & 0x2) diff += (step >> 1);
            if (delta & 0x1) diff += (step >> 2);
            if (sign) predictor -= diff;
            else predictor += diff;
            step = ima4_step_table[step_index];

            if (predictor > INT16_MAX) {
                *out++ = INT16_MAX;
            }
            else if (predictor < INT16_MIN) {
                *out++ = INT16_MIN;
            }
            else {
                *out++ = static_cast<int16_t>(predictor);
            }

            // decode the lower nibble
            step_index += ima4_index_table[upper_nibble];
            sign = lower_nibble & 0x8;
            delta = lower_nibble & 0x7;
            diff = step >> 8;
            if (delta & 0x4) diff += step;
            if (delta & 0x2) diff += (step >> 1);
            if (delta & 0x1) diff += (step >> 2);
            if (sign) predictor -= diff;
            else predictor += diff;
            step = ima4_step_table[step_index];

            if (predictor > INT16_MAX) {
                *out++ = INT16_MAX;
            }
            else if (predictor < INT16_MIN) {
                *out++ = INT16_MIN;
            }
            else {
                *out++ = static_cast<int16_t>(predictor);
            }
        }
    }

    chunk->bytes_per_packet = 128;
    chunk->bit_width = 16;
    chunk->frames_per_packet = 1;
    chunk->bytes_per_frame = (chunk->bit_width >> 3) * chunk->channels;
    chunk->format_id = 'lpcm';
    chunk->format_flags = 0x4;

    return chunk;
}