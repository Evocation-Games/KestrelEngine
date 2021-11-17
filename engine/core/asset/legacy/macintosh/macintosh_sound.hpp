// Copyright (c) 2020 Tom Hancocks
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

#include "core/asset/rsrc/resource_descriptor.hpp"
#include "scripting/state.hpp"

namespace asset
{

    struct macintosh_sound: public scripting::lua::object
    {
    public:
        constexpr static const char *type { "snd " };
        typedef luabridge::RefCountedPtr<asset::macintosh_sound> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit macintosh_sound(const asset::resource_descriptor::lua_reference& ref);
        ~macintosh_sound();

        lua_api static auto load(const asset::resource_descriptor::lua_reference& ref) -> macintosh_sound::lua_reference;
        lua_api virtual auto play() -> void;

        auto raw_data() const -> void *;
        auto raw_data_size() const -> uint32_t;
        auto channels() const -> uint32_t;
        auto samples() const -> uint32_t;
        auto sample_rate() const -> uint32_t;
        auto bits_per_channel() const -> uint32_t;
        auto format() const -> uint32_t;
        auto format_flags() const -> uint32_t;
        auto bytes_per_frame() const -> uint32_t;
        auto frames_per_packet() const -> uint32_t;
        auto bytes_per_packet() const -> uint32_t;

    private:
        static constexpr uint32_t rate_48khz = 0xbb800000;
        static constexpr uint32_t rate_44khz = 0xac440000;
        static constexpr uint32_t rate_32khz = 0x7d000000;
        static constexpr uint32_t rate_22050hz = 0x56220000;
        static constexpr uint32_t rate_22khz = 0x56ee8ba3;
        static constexpr uint32_t rate_16khz = 0x3e800000;
        static constexpr uint32_t rate_11khz = 0x2b7745d1;
        static constexpr uint32_t rate_11025hz = 0x2b110000;
        static constexpr uint32_t rate_8khz = 0x1f400000;
        
        static constexpr uint16_t sampled_synth = 5; // sampled sound synthesizer*/
        
        static constexpr uint8_t middle_c = 60; // MIDI note value for middle C*/
        
        static constexpr uint16_t data_offset_flag = 0x8000;
        
        static constexpr int16_t not_compressed = 0;    /*compression ID's*/
        static constexpr int16_t fixed_compression = -1;   /*compression ID for fixed-sized compression*/
        static constexpr int16_t variable_compression = -2;   /*compression ID for variable-sized compression*/
        static constexpr int16_t two_to_one = 1;
        static constexpr int16_t eight_to_three = 2;
        static constexpr int16_t three_to_one = 3;
        static constexpr int16_t six_to_one = 4;
        static constexpr int16_t six_to_one_packet_size = 8;
        static constexpr int16_t three_to_one_packet_size = 16;
        
        static constexpr int16_t first_sound_format = 0x0001; // general sound format*/
        static constexpr int16_t second_sound_format = 0x0002; // special sampled sound format (HyperCard)*/
        
        static constexpr uint8_t standard_header = 0x00;  // Standard sound header encode value*/
        static constexpr uint8_t extended_header = 0xFF;  // Extended sound header encode value*/
        static constexpr uint8_t compressed_header = 0xFE;  // Compressed sound header encode value*/
        
        // command numbers for SndDoCommand and SndDoImmediate
        static constexpr uint16_t null_cmd = 0;
        static constexpr uint16_t quiet_cmd = 3;
        static constexpr uint16_t flush_cmd = 4;
        static constexpr uint16_t re_init_cmd = 5;
        static constexpr uint16_t wait_cmd = 10;
        static constexpr uint16_t pause_cmd = 11;
        static constexpr uint16_t resume_cmd = 12;
        static constexpr uint16_t call_back_cmd = 13;
        static constexpr uint16_t sync_cmd = 14;
        static constexpr uint16_t available_cmd = 24;
        static constexpr uint16_t version_cmd = 25;
        static constexpr uint16_t volume_cmd = 46;   // sound manager 3.0 or later only
        static constexpr uint16_t get_volume_cmd = 47;   // sound manager 3.0 or later only
        static constexpr uint16_t clock_component_cmd = 50;   // sound manager 3.2.1 or later only
        static constexpr uint16_t get_clock_component_cmd = 51;   // sound manager 3.2.1 or later only
        static constexpr uint16_t scheduled_sound_cmd = 52;   // sound manager 3.3 or later only
        static constexpr uint16_t link_sound_components_cmd = 53;   // sound manager 3.3 or later only
        static constexpr uint16_t sound_cmd = 80;
        static constexpr uint16_t buffer_cmd = 81;
        static constexpr uint16_t rate_multiplier_cmd = 86;
        static constexpr uint16_t get_rate_multiplier_cmd = 87;
        
        static constexpr int32_t init_chan_left = 0x0002; // left stereo channel
        static constexpr int32_t init_chan_right = 0x0003; // right stereo channel
        static constexpr int32_t init_no_interp = 0x0004; // no linear interpolation
        static constexpr int32_t init_no_drop = 0x0008; // no drop-sample conversion
        static constexpr int32_t init_mono = 0x0080; // monophonic channel
        static constexpr int32_t init_stereo = 0x00C0; // stereo channel
        static constexpr int32_t init_m_a_c_e3 = 0x0300; // MACE 3:1
        static constexpr int32_t init_m_a_c_e6 = 0x0400; // MACE 6:1
        
        // Format Types
        static constexpr uint32_t sound_format_not_compressed = 0x4E4F4E45; // 'NONE' sound is not compressed
        static constexpr uint32_t sound_format_8_bit_offset = 0x72617720; // 'raw ' 8-bit offset binary
        static constexpr uint32_t sound_format_16_bit_be = 0x74776F73; // 'twos' 16-bit big endian
        static constexpr uint32_t sound_format_16_bit_le = 0x736F7774; // 'sowt' 16-bit little endian
        static constexpr uint32_t sound_format_ima4 = 'ima4';

        struct sound_command { uint16_t cmd; int16_t param1; int32_t param2; };
        struct mod_ref { uint16_t mod_number; int32_t mod_init; };

        struct sound_list_resource
        {
            int16_t format;
            int16_t modifier_count;
            struct mod_ref modifier_part;
            int16_t command_count;
            struct sound_command command_part;
        };

        struct hypercard_sound_list_resource
        {
            int16_t format;
            int16_t ref_count;
            int16_t command_count;
            struct sound_command command_part;
        };

        struct sound_header
        {
            uint32_t sample_ptr;
            uint32_t length;
            uint32_t sample_rate_fixed;
            uint32_t loop_start;
            uint32_t loop_end;
            uint8_t encode;
            uint8_t base_freq;
        };

        struct compressed_sound_header
        {
            uint32_t frame_count;
            int16_t aiff_sample_rate_exp;
            uint64_t aiff_sample_rate_man;
            uint32_t marker_chunk;
            uint32_t format;
            uint32_t future_use;
            uint32_t state_vars;
            uint32_t left_over_samples;
            uint16_t compression_id;
            uint16_t packet_size;
            uint16_t synth_id;
            uint16_t sample_size;
        };

        struct extended_sound_header
        {
            uint32_t frame_count;
            int16_t aiff_sample_rate_exp;
            uint64_t aiff_sample_rate_man;
            uint32_t marker_chunk;
            uint32_t instrument_chunks;
            uint32_t aes_recording;
            uint16_t sample_size;
        };

        uint32_t m_sample_rate { 0 };
        uint32_t m_channels_per_frame { 0 };
        uint32_t m_format_id { 0 };
        uint32_t m_format_flags { 0 };
        uint8_t m_bits_per_channel { 8 };
        uint32_t m_bytes_per_frame { 0 };
        uint32_t m_frames_per_packet { 0 };
        uint32_t m_bytes_per_packet { 0 };
        uint32_t m_packet_count { 0 };
        void *m_samples { nullptr };
        uint32_t m_samples_size { 0 };

        auto parse(const std::shared_ptr<graphite::data::data>& data) -> bool;

    };

};

#endif //KESTREL_SOUND_HPP
