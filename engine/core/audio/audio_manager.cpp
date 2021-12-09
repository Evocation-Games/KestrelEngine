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

#include <algorithm>
#include <utility>
#include "core/audio/audio_manager.hpp"

// MARK: - Manager

auto audio::manager::shared_manager() -> manager &
{
    static audio::manager instance;
    return instance;
}

auto audio::manager::construct_playback_reference(std::shared_ptr<audio::chunk> chunk, std::function<auto()->void> completion) -> playback_reference *
{
    playback_reference ref(m_next_ref_id++, std::move(chunk), std::move(completion));
    m_playback_refs.emplace_back(ref);
    return &m_playback_refs.back();
}

auto audio::manager::monitor_finished_playbacks() -> void
{
    if (m_playback_refs.empty()) {
        return;
    }

    m_playback_refs.erase(std::remove_if(m_playback_refs.begin(), m_playback_refs.end(), [](const playback_reference& ref) {
        if (ref.finished) {
            ref.completion_callback();
            return true;
        }
        else {
            return false;
        }
    }), m_playback_refs.end());
}

auto audio::manager::finish_playback(uint64_t playback_id) -> void
{
    for (auto& ref : m_playback_refs) {
        if (ref.id == playback_id) {
            ref.finished = true;
            return;
        }
    }
}

// MARK: - Configuration

#if __APPLE__
#   include "core/audio/core_audio.hpp"
#endif

#include "core/audio/openal.hpp"

auto audio::manager::set_openal(bool al) -> void
{
    m_use_openal = al;
}

auto audio::manager::using_openal() const -> bool
{
    return m_use_openal;
}

auto audio::manager::configure() -> void
{
    if (m_use_openal) {
        openal::player::global().configure_devices();
    }
}

auto audio::manager::play(std::shared_ptr<audio::chunk> chunk, std::function<auto()->void> completion) -> void
{
    auto ref = construct_playback_reference(std::move(chunk), std::move(completion));
    if (m_use_openal) {
        openal::player::global().play(ref);
    }
    else {
        core_audio::play(ref);
    }

}

auto audio::manager::play_background_audio(const std::string& path) -> void
{
    if (m_use_openal) {
        openal::player::global().play_background_audio(path);
    }
    else {
        core_audio::play_background_audio(path);
    }
}

auto audio::manager::stop_background_audio() -> void
{
    if (m_use_openal) {
        openal::player::global().stop_background_audio();
    }
    else {
        core_audio::stop_background_audio();
    }
}
