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

#pragma once

#include <libKestrel/util/availability.hpp>

#if TARGET_MACOS
#   include <OpenAL/al.h>
#   include <OpenAL/alc.h>
#else
#   include <AL/al.h>
#   include <AL/alc.h>
#endif

#include <libKestrel/sound/player/player.hpp>

namespace kestrel::sound::openal
{

    struct playback_session_info
    {
        ALuint handle { 0 };
        ALuint source { 0 };
        ALenum format { AL_FORMAT_MONO8 };
    };

    class player : public sound::player<playback_session_info>
    {
    private:
        ALCcontext *m_context { nullptr };
        ALCdevice *m_device { nullptr };
        ALCboolean m_context_current { false };
        bool m_configured { false };

    public:
        auto configure() -> void override;

        auto check_completion() -> void override;
        auto play(std::shared_ptr<sound::player_item> item, std::function<auto()->void> finished) -> playback_session_ref override;
        auto stop(const playback_session_ref& ref) -> void override;

        auto acquire_player_info() -> playback_session_info override;
        auto configure_playback_session(std::shared_ptr<sound::playback_session<playback_session_info>> session) -> void override;

        static auto check_errors(const std::string& filename, std::uint_fast32_t line) -> bool;
        static auto check_errors(const std::string& filename, std::uint_fast32_t line, ALCdevice *dev) -> bool;

    };

}
