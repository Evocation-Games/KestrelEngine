//
// Created by tomhancocks on 19/11/2021.
//

#include <cstdlib>
#include <cstring>
#include "core/audio/sound.hpp"

// MARK: - Sound Packet

audio::sound::packet::packet(uint32_t sample_rate, uint8_t channels, uint8_t bits, const void *bytes, const uint32_t length)
{
    this->sample_rate = sample_rate;
    this->channels = channels;
    this->bits = bits;
    this->length = length;
    this->bytes = malloc(this->length);
    memcpy(this->bytes, bytes, this->length);
}

audio::sound::packet::~packet()
{
    free(this->bytes);
    this->bytes = nullptr;
}

// MARK: - Sound

audio::sound::sound(audio::sound& snd)
    : m_channels(snd.m_channels),
    m_playback_packet_idx(0),
    m_sample_size(snd.m_sample_size),
    m_sample_rate(snd.m_sample_rate)
{
    for (const auto& p : snd.m_packets) {
        m_packets.emplace_back(p.sample_rate, p.channels, p.bits, p.bytes, p.length);
    }
}

audio::sound::sound(uint32_t sample_rate, uint8_t sample_size, uint8_t channels)
    : m_channels(channels), m_sample_rate(sample_rate), m_sample_size(sample_size)
{

}

auto audio::sound::add_packet(const void *bytes, uint32_t length) -> void
{
    m_packets.emplace_back( m_sample_rate, m_channels, m_sample_size, bytes, length );
}

auto audio::sound::valid() const -> bool
{
    return !m_packets.empty();
}

// MARK: - Sound Playback

auto audio::sound::reset_playback() -> void
{
    m_playback_packet_idx = 0;
}

auto audio::sound::playback_current_packet_idx() const -> uint32_t
{
    return m_playback_packet_idx;
}

auto audio::sound::playback_current_packet() const -> const packet &
{
    return m_packets.at(m_playback_packet_idx);
}

auto audio::sound::playback_consume_packet() -> const packet &
{
    auto packet = playback_current_packet();
    m_playback_packet_idx++;
    return packet;
}