//
// Created by Tom Hancocks on 13/11/2021.
//

#if !defined(AUDIO_MANAGER_HPP)
#define AUDIO_MANAGER_HPP

#include <vector>
#include "core/audio/sound.hpp"
#include "core/asset/legacy/macintosh/macintosh_sound.hpp"

namespace audio
{

    class manager
    {
    private:
        manager() = default;

    public:
        manager(const manager&) = delete;
        manager& operator=(const manager &) = delete;
        manager(manager &&) = delete;
        manager & operator=(manager &&) = delete;

        static auto shared_manager() -> manager&;

        auto configure() -> void;

        auto play(audio::sound snd) -> void;
    };

    auto play_sound(const asset::macintosh_sound& snd) -> void;

}

#endif //AUDIO_MANAGER_HPP
