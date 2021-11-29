//
// Created by Tom Hancocks on 23/11/2021.
//

#if !defined(CORE_AUDIO_HPP) && __APPLE__
#define CORE_AUDIO_HPP

#include <string>
#include <memory>
#include <functional>
#include "core/audio/chunk.hpp"
#include "core/audio/audio_manager.hpp"

namespace audio::core_audio
{
    auto play(audio::manager::playback_reference *ref) -> void;

    auto play_background_audio(const std::string& path) -> void;
    auto stop_background_audio() -> void;
}

#endif //CORE_AUDIO_HPP
