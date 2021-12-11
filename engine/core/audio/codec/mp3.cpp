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

#include "core/audio/codec/audio_codec_descriptor.hpp"
#include "core/audio/codec/mp3.hpp"
#include "core/audio/audio_manager.hpp"

#define MINIMP3_ONLY_MP3
#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

// MARK: - Lua

auto audio::asset::mp3::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Audio")
            .beginClass<audio::asset::mp3>("MP3")
                .addConstructor<auto(*)(const std::string&)->void, audio::asset::mp3::lua_reference>()
                .addFunction("play", &audio::asset::mp3::play)
                .addFunction("stop", &audio::asset::mp3::stop)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

audio::asset::mp3::mp3(const std::string& file_path)
    : m_path(file_path)
{
    mp3dec_t mp3d;
    mp3dec_file_info_t info;
    if (mp3dec_load(&mp3d, file_path.c_str(), &info, nullptr, nullptr)) {
        // TODO: Handle error case...
        return;
    }

    codec_descriptor descriptor;
    descriptor.channels = info.channels;
    descriptor.bit_width = 16;
    descriptor.sample_rate = info.hz;

    if (audio::manager::shared_manager().current_library() == audio::manager::library::core_audio) {
        m_item = std::make_shared<audio::player_item>(m_path);
    }
    else {
        m_item = std::make_shared<audio::player_item>(descriptor, info.buffer, (info.samples << 1));
    }
}

// MARK: - Playback

auto audio::asset::mp3::play() -> void
{
    m_ref = audio::manager::shared_manager().play_item(m_item, [&] {
        this->m_ref = 0;
    });
}

auto audio::asset::mp3::stop() -> void
{
    audio::manager::shared_manager().stop_item(m_ref);
    m_ref = 0;
}
