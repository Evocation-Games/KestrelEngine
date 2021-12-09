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

#if !defined(KESTREL_OPENAL_HPP)
#define KESTREL_OPENAL_HPP

#if __APPLE__
#   include <OpenAL/al.h>
#   include <OpenAL/alc.h>
#else
#   include <AL/al.h>
#   include <AL/alc.h>
#endif

#include <string>
#include <thread>
#include <memory>
#include <vector>
#include "core/audio/chunk.hpp"

namespace audio::openal
{

    class player
    {
    private:
        struct playback_buffer
        {
            ALuint handle { 0 };
            ALuint source { 0 };
            ALenum format { AL_FORMAT_MONO8 };
            std::shared_ptr<audio::chunk> chunk { nullptr };

            playback_buffer() = default;
        };

        player();

        bool m_configured { false };
        bool m_configuration_attempted { false };
        ALCcontext *m_context { nullptr };
        ALCdevice *m_device { nullptr };
        ALCboolean m_context_current { false };
        std::vector<std::unique_ptr<playback_buffer>> m_buffers;
        std::unique_ptr<std::thread> m_playback_thread;

        auto playback_loop() -> void;

    public:
        player(const player&) = delete;
        player& operator=(const player&) = delete;
        player(player&&) = delete;
        player& operator=(player&&) = delete;

        static auto global() -> player&;

        auto configure_devices() -> bool;

        auto play(std::shared_ptr<audio::chunk> chunk) -> bool;

        static auto check_errors(const std::string& filename, uint_fast32_t line) -> bool;
        static auto check_errors(const std::string& filename, uint_fast32_t line, ALCdevice *dev) -> bool;

        auto play_background_audio(const std::string& path) -> void;
        auto stop_background_audio() -> void;
    };

}

#endif //KESTREL_OPENAL_HPP
