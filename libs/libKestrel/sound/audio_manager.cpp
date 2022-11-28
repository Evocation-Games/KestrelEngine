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
#include <libKestrel/sound/audio_manager.hpp>
#include <libKestrel/exceptions/incompatible_driver_exception.hpp>

// MARK: - Manager

auto kestrel::sound::manager::shared_manager() -> manager&
{
    static kestrel::sound::manager instance;
    return instance;
}

// MARK: - Library Management

auto kestrel::sound::manager::set_api(enum api api) -> void
{
    if (api == m_api) {
        return;
    }

    // Release the previous player
#if TARGET_MACOS
    m_core_audio.reset();
#endif
    m_openal.reset();

    // Assign the player...
    m_api = api;
    switch (m_api) {
        case api::core_audio: {
#if TARGET_MACOS
            m_core_audio = std::make_shared<sound::core_audio::player>();
            m_core_audio->configure();
            break;
#else
            throw incompatible_driver_exception("CoreAudio is not available on this system.");
#endif
        }
        case api::openal: {
            m_openal = std::make_shared<openal::player>();
            m_openal->configure();
            break;
        }
        case api::none: {
            break;
        }
    }
}

auto kestrel::sound::manager::current_api() const -> enum api
{
    return m_api;
}

// MARK: - Playback

auto kestrel::sound::manager::play_item(std::shared_ptr<player_item> item, std::function<auto()->void> completion) -> playback_session_ref
{
    switch (m_api) {
        case api::core_audio:
#if TARGET_MACOS
            return m_core_audio->play(std::move(item), std::move(completion));
#else
            return 0;
#endif

        case api::openal:
            return m_openal->play(std::move(item), std::move(completion));

        default:
            return 0;
    }
}

auto kestrel::sound::manager::stop_item(const playback_session_ref& ref) -> void
{
    switch (m_api) {
        case api::core_audio:
#if TARGET_MACOS
            return m_core_audio->stop(ref);
#else
            return;
#endif

        case api::openal:
            return m_openal->stop(ref);

        default:
            return;
    }
}

auto kestrel::sound::manager::finish_item(const playback_session_ref &ref) -> void
{
    switch (m_api) {
        case api::core_audio:
#if TARGET_MACOS
            m_core_audio->stop(ref);
#else
            return;
#endif

        case api::openal:
            m_openal->stop(ref);

        default:
            return;
    }
}

// MARK: - Timings

auto kestrel::sound::manager::tick() -> void
{
    // The purpose of this tick, is to check for completed sounds when using the OpenAL engine.
    // Given that we don't receive a notification for when the sound finishes playing, we need to
    // periodically check to see if the sound has finished playing.
    switch (m_api) {
        case api::core_audio:
#if TARGET_MACOS
            m_core_audio->check_completion();
#endif
            break;

        case api::openal:
            m_openal->check_completion();
            break;

        default:
            break;
    }
}
