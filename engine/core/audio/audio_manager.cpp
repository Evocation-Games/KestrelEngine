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

// MARK: - Library Management

auto audio::manager::set_library(library lib) -> void
{
    if (lib == m_library) {
        return;
    }

    // Release the previous player
    m_core_audio.reset();

    // Assign the player...
    m_library = lib;
    switch (m_library) {
        case library::core_audio: {
#if __APPLE__
            m_core_audio = std::make_shared<audio::core_audio::player>();
            m_core_audio->configure();
            break;
#endif
        }
        case library::openal: {
            m_openal = std::make_shared<audio::openal::player>();
            m_openal->configure();
            break;
        }
        case library::none: {
            break;
        }
    }
}

auto audio::manager::current_library() const -> library
{
    return m_library;
}

// MARK: - Playback

auto audio::manager::play_item(std::shared_ptr<player_item> item, std::function<auto()->void> completion) -> playback_session_ref
{
    switch (m_library) {
        case library::core_audio:
#if __APPLE__
            return m_core_audio->play(std::move(item), std::move(completion));
#endif

        case library::openal:
            return m_openal->play(std::move(item), std::move(completion));

        default:
            return 0;
    }
}

auto audio::manager::stop_item(const audio::playback_session_ref& ref) -> void
{
    switch (m_library) {
        case library::core_audio:
#if __APPLE__
            return m_core_audio->stop(ref);
#endif

        case library::openal:
            return m_openal->stop(ref);

        default:
            return;
    }
}

auto audio::manager::finish_item(const playback_session_ref &ref) -> void
{
    switch (m_library) {
        case library::core_audio:
#if __APPLE__
            return m_core_audio->stop(ref);
#endif

        case library::openal:
            return m_openal->stop(ref);

        default:
            return;
    }
}

// MARK: - Timings

auto audio::manager::tick() -> void
{
    // The purpose of this tick, is to check for completed sounds when using the OpenAL engine.
    // Given that we don't receive a notification for when the sound finishes playing, we need to
    // periodically check to see if the sound has finished playing.
    switch (m_library) {
        case library::core_audio:
            m_core_audio->check_completion();
            break;

        case library::openal:
            m_openal->check_completion();
            break;

        default:
            break;
    }
}
