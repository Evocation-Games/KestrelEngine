//
// Created by tomhancocks on 19/11/2021.
//

#if !defined(KESTREL_OPENAL_HPP) && !__APPLE__
#define KESTREL_OPENAL_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <thread>
#include <memory>
#include "core/audio/sound.hpp"

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
            audio::sound sound;
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

        auto play(audio::sound snd) -> bool;

        static auto check_errors(const std::string& filename, uint_fast32_t line) -> bool;
        static auto check_errors(const std::string& filename, uint_fast32_t line, ALCdevice *dev) -> bool;
    };

}

#endif //KESTREL_OPENAL_HPP
