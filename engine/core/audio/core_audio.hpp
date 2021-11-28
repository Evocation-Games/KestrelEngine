//
// Created by Tom Hancocks on 23/11/2021.
//

#if !defined(CORE_AUDIO_HPP)
#define CORE_AUDIO_HPP

#include <memory>
#include <functional>
#include "core/audio/chunk.hpp"
#include "core/audio/audio_manager.hpp"

namespace audio::core_audio
{
    auto play(audio::manager::playback_reference *ref) -> void;
}

#endif //CORE_AUDIO_HPP
