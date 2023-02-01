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

#include <stdexcept>
#include <libGraphite/data/reader.hpp>
#include <libKestrel/sound/legacy/macintosh/sound.hpp>
#include <libKestrel/cache/cache.hpp>
#include <libKestrel/sound/audio_manager.hpp>
#include <libKestrel/sound/codec/audio_codec_descriptor.hpp>
#include <libKestrel/sound/codec/ima4.hpp>
#include <libKestrel/kestrel.hpp>

// MARK: - Constants

static constexpr std::uint32_t rate_48khz = 0xbb800000;
static constexpr std::uint32_t rate_44khz = 0xac440000;
static constexpr std::uint32_t rate_32khz = 0x7d000000;
static constexpr std::uint32_t rate_22050hz = 0x56220000;
static constexpr std::uint32_t rate_22khz = 0x56ee8ba3;
static constexpr std::uint32_t rate_16khz = 0x3e800000;
static constexpr std::uint32_t rate_11khz = 0x2b7745d1;
static constexpr std::uint32_t rate_11025hz = 0x2b110000;
static constexpr std::uint32_t rate_8khz = 0x1f400000;

static constexpr std::uint16_t sampled_synth = 5; // sampled sound synthesizer*/

static constexpr std::uint8_t middle_c = 60; // MIDI note value for middle C*/

static constexpr std::uint16_t data_offset_flag = 0x8000;

static constexpr std::int16_t not_compressed = 0;    /*compression ID's*/
static constexpr std::int16_t fixed_compression = -1;   /*compression ID for fixed-sized compression*/
static constexpr std::int16_t variable_compression = -2;   /*compression ID for variable-sized compression*/
static constexpr std::int16_t two_to_one = 1;
static constexpr std::int16_t eight_to_three = 2;
static constexpr std::int16_t three_to_one = 3;
static constexpr std::int16_t six_to_one = 4;
static constexpr std::int16_t six_to_one_packet_size = 8;
static constexpr std::int16_t three_to_one_packet_size = 16;

static constexpr std::int16_t first_sound_format = 0x0001; // general sound format*/
static constexpr std::int16_t second_sound_format = 0x0002; // special sampled sound format (HyperCard)*/

static constexpr std::uint8_t standard_header = 0x00;  // Standard sound header encode value*/
static constexpr std::uint8_t extended_header = 0xFF;  // Extended sound header encode value*/
static constexpr std::uint8_t compressed_header = 0xFE;  // Compressed sound header encode value*/

// command numbers for SndDoCommand and SndDoImmediate
static constexpr std::uint16_t null_cmd = 0;
static constexpr std::uint16_t quiet_cmd = 3;
static constexpr std::uint16_t flush_cmd = 4;
static constexpr std::uint16_t re_init_cmd = 5;
static constexpr std::uint16_t wait_cmd = 10;
static constexpr std::uint16_t pause_cmd = 11;
static constexpr std::uint16_t resume_cmd = 12;
static constexpr std::uint16_t call_back_cmd = 13;
static constexpr std::uint16_t sync_cmd = 14;
static constexpr std::uint16_t available_cmd = 24;
static constexpr std::uint16_t version_cmd = 25;
static constexpr std::uint16_t volume_cmd = 46;   // sound manager 3.0 or later only
static constexpr std::uint16_t get_volume_cmd = 47;   // sound manager 3.0 or later only
static constexpr std::uint16_t clock_component_cmd = 50;   // sound manager 3.2.1 or later only
static constexpr std::uint16_t get_clock_component_cmd = 51;   // sound manager 3.2.1 or later only
static constexpr std::uint16_t scheduled_sound_cmd = 52;   // sound manager 3.3 or later only
static constexpr std::uint16_t link_sound_components_cmd = 53;   // sound manager 3.3 or later only
static constexpr std::uint16_t sound_cmd = 80;
static constexpr std::uint16_t buffer_cmd = 81;
static constexpr std::uint16_t rate_multiplier_cmd = 86;
static constexpr std::uint16_t get_rate_multiplier_cmd = 87;

static constexpr std::int32_t init_chan_left = 0x0002; // left stereo channel
static constexpr std::int32_t init_chan_right = 0x0003; // right stereo channel
static constexpr std::int32_t init_no_interp = 0x0004; // no linear interpolation
static constexpr std::int32_t init_no_drop = 0x0008; // no drop-sample conversion
static constexpr std::int32_t init_mono = 0x0080; // monophonic channel
static constexpr std::int32_t init_stereo = 0x00C0; // stereo channel
static constexpr std::int32_t init_m_a_c_e3 = 0x0300; // MACE 3:1
static constexpr std::int32_t init_m_a_c_e6 = 0x0400; // MACE 6:1

// Format Types
static constexpr std::uint32_t sound_format_not_compressed = 0x4E4F4E45; // 'NONE' sound is not compressed
static constexpr std::uint32_t sound_format_8_bit_offset = 0x72617720; // 'raw ' 8-bit offset binary
static constexpr std::uint32_t sound_format_16_bit_be = 0x74776F73; // 'twos' 16-bit big endian
static constexpr std::uint32_t sound_format_16_bit_le = 0x736F7774; // 'sowt' 16-bit little endian
static constexpr std::uint32_t sound_format_ima4 = 'ima4';

struct sound_command { std::uint16_t cmd; std::int16_t param1; std::int32_t param2; };
struct mod_ref { std::uint16_t mod_number; std::int32_t mod_init; };

struct sound_list_resource
{
    std::int16_t format;
    std::int16_t modifier_count;
    struct mod_ref modifier_part;
    std::int16_t command_count;
    struct sound_command command_part;
};

struct hypercard_sound_list_resource
{
    std::int16_t format;
    std::int16_t ref_count;
    std::int16_t command_count;
    struct sound_command command_part;
};

struct sound_header
{
    std::uint32_t sample_ptr;
    std::uint32_t length;
    std::uint32_t sample_rate_fixed;
    std::uint32_t loop_start;
    std::uint32_t loop_end;
    std::uint8_t encode;
    std::uint8_t base_freq;
};

struct compressed_sound_header
{
    std::uint32_t frame_count;
    std::int16_t aiff_sample_rate_exp;
    std::uint64_t aiff_sample_rate_man;
    std::uint32_t marker_chunk;
    std::uint32_t format;
    std::uint32_t future_use;
    std::uint32_t state_vars;
    std::uint32_t left_over_samples;
    std::uint16_t compression_id;
    std::uint16_t packet_size;
    std::uint16_t synth_id;
    std::uint16_t sample_size;
};

struct extended_sound_header
{
    std::uint32_t frame_count;
    std::int16_t aiff_sample_rate_exp;
    std::uint64_t aiff_sample_rate_man;
    std::uint32_t marker_chunk;
    std::uint32_t instrument_chunks;
    std::uint32_t aes_recording;
    std::uint16_t sample_size;
};

// MARK: - Construction

kestrel::sound::legacy::macintosh::quicktime::sound::sound(const resource::descriptor::lua_reference& ref)
{
    if (auto res = ref->with_type(resource_type::code)->load()) {
        parse(res->data());
        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto kestrel::sound::legacy::macintosh::quicktime::sound::load(const resource::descriptor::lua_reference& ref) -> lua_reference
{
    // Attempt to de-cache asset
    auto asset = cache::fetch(ref->with_type(resource_type::code));
    if (asset.has_value()) {
        return std::any_cast<lua_reference>(asset.value());
    }

    auto snd = lua_reference(new sound(ref));
    cache::add(ref->with_type(resource_type::code), snd);
    return snd;
}


// MARK: - Playback

auto kestrel::sound::legacy::macintosh::quicktime::sound::play() -> void
{
    if (m_item != nullptr) {
        m_item_reference = manager::shared_manager().play_item(m_item, [&] {
            this->m_item_reference = 0;
        });
    }
}

auto kestrel::sound::legacy::macintosh::quicktime::sound::playWithCallback(const luabridge::LuaRef& ref) -> void
{
    if (m_item != nullptr) {
        m_item_reference = manager::shared_manager().play_item(m_item, [&, ref] {
            this->m_item_reference = 0;
            ref();
        });
    }
}

auto kestrel::sound::legacy::macintosh::quicktime::sound::stop() -> void
{
    if (m_item != nullptr) {
        manager::shared_manager().stop_item(m_item_reference);
    }
}

// MARK: - Sound Resource Parsing

auto kestrel::sound::legacy::macintosh::quicktime::sound::parse(const graphite::data::block &data) -> bool
{
    codec::descriptor descriptor;
    graphite::data::reader r(&data);
    auto sound_format = r.read_signed_short();

    if (sound_format == first_sound_format) {
        sound_list_resource list;
        list.format = sound_format;
        list.modifier_count = r.read_signed_short();
        list.modifier_part.mod_number = r.read_signed_short();
        list.modifier_part.mod_init = r.read_signed_long();
        list.command_count = r.read_signed_short();
        list.command_part.cmd = r.read_short();
        list.command_part.param1 = r.read_signed_short();
        list.command_part.param2 = r.read_signed_long();

        if (list.modifier_count != 1
         || list.modifier_part.mod_number != sampled_synth
         || list.command_count != 1
         || list.command_part.cmd != (data_offset_flag + buffer_cmd)
        ) {
            // TODO: Throw an error here.
            return false;
        }
    }
    else if (sound_format == second_sound_format) {
        hypercard_sound_list_resource list;
        list.format = sound_format;
        list.ref_count = r.read_signed_short();
        list.command_count = r.read_signed_short();
        list.command_part.cmd = r.read_short();
        list.command_part.param1 = r.read_signed_short();
        list.command_part.param2 = r.read_signed_long();

        if (list.command_count != 1 || list.command_part.cmd != (data_offset_flag + buffer_cmd)) {
            // TODO: Throw an error here.
            return false;
        }
    }
    else {
        // TODO: Throw an error here.
        return false;
    }

    sound_header header;
    header.sample_ptr = r.read_long();
    header.length = r.read_long();
    header.sample_rate_fixed = r.read_long();
    header.loop_start = r.read_long();
    header.loop_end = r.read_long();
    header.encode = r.read_byte();
    header.base_freq = r.read_byte();

    if (header.encode == standard_header) {
        descriptor.format_id = sound_format_8_bit_offset;
        descriptor.channels = 1;
        descriptor.packet_count = header.length;
    }
    else if (header.encode == extended_header) {
        extended_sound_header ext;
        ext.frame_count = r.read_long();
        ext.aiff_sample_rate_exp = r.read_signed_short();
        ext.aiff_sample_rate_man = r.read_quad();
        ext.marker_chunk = r.read_long();
        ext.instrument_chunks = r.read_long();
        ext.aes_recording = r.read_long();
        ext.sample_size = r.read_short();
        r.move(14);

        descriptor.format_id = ext.sample_size == 8 ? sound_format_8_bit_offset : sound_format_16_bit_be;
        descriptor.channels = header.length;
        descriptor.packet_count = ext.frame_count;
    }
    else if (header.encode == compressed_header) {
        compressed_sound_header cmp;
        cmp.frame_count = r.read_long();
        cmp.aiff_sample_rate_exp = r.read_signed_short();
        cmp.aiff_sample_rate_man = r.read_quad();
        cmp.marker_chunk = r.read_long();
        cmp.format = r.read_long();
        cmp.future_use = r.read_long();
        cmp.state_vars = r.read_long();
        cmp.left_over_samples = r.read_long();
        cmp.compression_id = r.read_short();
        cmp.packet_size = r.read_short();
        cmp.synth_id = r.read_short();
        cmp.sample_size = r.read_short();

        if (cmp.compression_id == three_to_one) {
            descriptor.format_id = 'MAC3';
        }
        else if (cmp.compression_id == six_to_one) {
            descriptor.format_id = 'MAC6';
        }
        else {
            descriptor.format_id = cmp.format;
        }

        descriptor.channels = header.length;
        descriptor.packet_count = cmp.frame_count;
    }
    else {
        return false;
    }

    descriptor.sample_rate = static_cast<std::uint32_t>(static_cast<double>(header.sample_rate_fixed) * 1.0 / static_cast<double>(1 << 16));

    if (descriptor.format_id == sound_format_8_bit_offset || descriptor.format_id == sound_format_16_bit_be) {
       if (descriptor.format_id == sound_format_8_bit_offset) {
           descriptor.bit_width = 8;
       }
       else {
           descriptor.bit_width = 16;
           descriptor.format_flags = 0x6; // kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsBigEndian
       }
       descriptor.format_id = 'lpcm';
       descriptor.bytes_per_frame = (descriptor.bit_width >> 3) * descriptor.channels;
       descriptor.frames_per_packet = 1;
       descriptor.bytes_per_packet = descriptor.bytes_per_frame * descriptor.frames_per_packet;

        m_item = std::make_shared<player_item>(descriptor);
        m_item->allocate_buffer(r.size() - r.position());
        auto ptr = reinterpret_cast<std::uint8_t *>(m_item->internal_buffer_pointer());
        for (int i = 0; i < m_item->buffer_size(); ++i) {
            *ptr++ = r.read_byte();
        }
    }
    else if (descriptor.format_id == sound_format_ima4) {
        // TODO: Do not hard code this, but work out the conversions...
        descriptor.format_flags = 0;
        descriptor.bytes_per_packet = 34;
        descriptor.frames_per_packet = 64;
        descriptor.bytes_per_frame = 0;
        descriptor.channels = 1;
        descriptor.bit_width = 0;

        switch (manager::shared_manager().current_api()) {
            case api::core_audio: {
                m_item = std::make_shared<player_item>(descriptor);
                m_item->allocate_buffer(r.size() - r.position());
                auto ptr = reinterpret_cast<std::uint8_t *>(m_item->internal_buffer_pointer());
                for (int i = 0; i < m_item->buffer_size(); ++i) {
                    *ptr++ = r.read_byte();
                }
                break;
            }
            default: {
                m_item = codec::ima4::decode(descriptor, r);
                break;
            }
        }
    }
    else {
        // TODO: Handle this correctly...
    }

    m_item->set_format(descriptor.format_id);
    m_item->set_format_flags(descriptor.format_flags);

    return true;
}

// MARK: - Accessors
