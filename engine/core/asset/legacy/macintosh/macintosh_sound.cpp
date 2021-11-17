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
#include "core/asset/legacy/macintosh/macintosh_sound.hpp"
#include "core/asset/cache.hpp"
#include "core/environment.hpp"
#include "core/audio/audio_manager.hpp"

// MARK: - Lua

auto asset::macintosh_sound::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<asset::macintosh_sound>("Sound")
                    .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&) -> void, asset::macintosh_sound::lua_reference>()
                    .addStaticFunction("load", &asset::macintosh_sound::load)
                    .addFunction("play", &asset::macintosh_sound::play)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::macintosh_sound::macintosh_sound(const asset::resource_descriptor::lua_reference& ref)
{
    if (auto res = ref->with_type(type)->load().lock()) {
        parse(res->data());
        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::macintosh_sound::load(const asset::resource_descriptor::lua_reference& ref) -> macintosh_sound::lua_reference
{
    // Attempt to de-cache asset
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(macintosh_sound::type, ref);
        if (asset.has_value()) {
            return std::any_cast<asset::macintosh_sound::lua_reference>(asset.value());
        }
    }

    auto sound = asset::macintosh_sound::lua_reference(new asset::macintosh_sound(ref));
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(macintosh_sound::type, ref, sound);
    }
    return sound;
}

// MARK: - Destruction

asset::macintosh_sound::~macintosh_sound()
{
    if (m_samples) {
        free(m_samples);
    }
}

// MARK: - Playback

auto asset::macintosh_sound::play() -> void
{
    if (m_samples == nullptr) {
        return;
    }
    audio::play_sound(*this);
}

// MARK: - Sound Resource Parsing

auto asset::macintosh_sound::parse(const std::shared_ptr<graphite::data::data> &data) -> bool
{
    graphite::data::reader r(data);
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

    uint32_t format = 0;
    if (header.encode == standard_header) {
        format = sound_format_8_bit_offset;
        m_channels_per_frame = 1;
        m_packet_count = header.length;
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

        format = ext.sample_size == 8 ? sound_format_8_bit_offset : sound_format_16_bit_be;
        m_channels_per_frame = header.length;
        m_packet_count = ext.frame_count;
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
            format = 'MAC3';
        }
        else if (cmp.compression_id == six_to_one) {
            format = 'MAC6';
        }
        else {
            format = cmp.format;
        }

        m_channels_per_frame = header.length;
        m_packet_count = cmp.frame_count;
    }
    else {
        return false;
    }

    m_sample_rate = static_cast<uint32_t>(static_cast<double>(header.sample_rate_fixed) * 1.0 / static_cast<double>(1 << 16));
    m_format_id = format;
    m_format_flags = 0;

    if (format == sound_format_8_bit_offset || format == sound_format_16_bit_be) {
        m_format_id = 'lpcm';
        if (format == sound_format_8_bit_offset) {
            m_bits_per_channel = 8;
        }
        else {
            m_bits_per_channel = 16;
            m_format_flags = 0x6; // kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsBigEndian
        }
        m_bytes_per_frame = (m_bits_per_channel >> 3) * m_channels_per_frame;
        m_frames_per_packet = 1;
        m_bytes_per_packet = m_bytes_per_frame * m_frames_per_packet;
    }
    else if (format == sound_format_ima4) {
        // TODO: Do not hard code this, but work out the conversions...
        m_format_flags = 0;
        m_bytes_per_packet = 34;
        m_frames_per_packet = 64;
        m_bytes_per_frame = 0;
        m_channels_per_frame = 1;
        m_bits_per_channel = 0;
    }
    else {
        // TODO: Handle this correctly...
    }

    // Build the data buffer for the sound, so that we can send it for playback.
    auto byte_size = m_packet_count * m_bytes_per_packet;
    m_samples_size = byte_size;
    m_samples = malloc(m_samples_size);
    auto ptr = reinterpret_cast<uint8_t *>(m_samples);
    for (int i = 0; i < byte_size; ++i) {
        *ptr++ = r.read_byte();
    }

    return true;
}

// MARK: - Accessors

auto asset::macintosh_sound::raw_data() const -> void *
{
    return m_samples;
}

auto asset::macintosh_sound::raw_data_size() const -> uint32_t
{
    return m_samples_size;
}

auto asset::macintosh_sound::channels() const -> uint32_t
{
    return m_channels_per_frame;
}

auto asset::macintosh_sound::samples() const -> uint32_t
{
    return m_packet_count;
}

auto asset::macintosh_sound::sample_rate() const -> uint32_t
{
    return m_sample_rate;
}

auto asset::macintosh_sound::bits_per_channel() const -> uint32_t
{
    return m_bits_per_channel;
}

auto asset::macintosh_sound::format() const -> uint32_t
{
    return m_format_id;
}

auto asset::macintosh_sound::format_flags() const -> uint32_t
{
    return m_format_flags;
}

auto asset::macintosh_sound::bytes_per_frame() const -> uint32_t
{
    return m_bytes_per_frame;
}

auto asset::macintosh_sound::frames_per_packet() const -> uint32_t
{
    return m_frames_per_packet;
}

auto asset::macintosh_sound::bytes_per_packet() const -> uint32_t
{
    return m_bytes_per_packet;
}
