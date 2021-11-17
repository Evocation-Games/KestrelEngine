//
// Created by Tom Hancocks on 13/11/2021.
//

#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <vector>
#include "core/asset/legacy/macintosh/macintosh_sound.hpp"

namespace audio
{

    auto play_sound(const asset::macintosh_sound& snd) -> void;

}

#endif //AUDIO_MANAGER_HPP
