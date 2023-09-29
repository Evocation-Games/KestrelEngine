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

#include <libKestrel/sound/codec/audio_codec_descriptor.hpp>
#include <libKestrel/sound/codec/mp3.hpp>
#include <libKestrel/sound/audio_manager.hpp>

#define MINIMP3_ONLY_MP3
#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

// MARK: - Construction

kestrel::sound::codec::mp3::mp3(const std::string& file_path)
    : m_path(file_path)
{
    mp3dec_t mp3d;
    mp3dec_file_info_t info;
    if (mp3dec_load(&mp3d, file_path.c_str(), &info, nullptr, nullptr)) {
        // TODO: Handle error case...
        return;
    }

    descriptor descriptor;
    descriptor.channels = info.channels;
    descriptor.bit_width = 16;
    descriptor.sample_rate = info.hz;

    if (sound::manager::shared_manager().current_api() == sound::api::core_audio) {
        m_item = std::make_shared<sound::player_item>(m_path);
    }
    else {
        m_item = std::make_shared<sound::player_item>(descriptor, info.buffer, (info.samples << 1));
    }
}

// MARK: - Playback

auto kestrel::sound::codec::mp3::play() -> void
{
    m_ref = sound::manager::shared_manager().play_item(m_item, [&] {
        this->m_ref = 0;
    });
}

auto kestrel::sound::codec::mp3::stop() -> void
{
    sound::manager::shared_manager().stop_item(m_ref);
    m_ref = 0;
}
