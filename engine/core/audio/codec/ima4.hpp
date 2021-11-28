//
// Created by Tom Hancocks on 21/11/2021.
//

#if !defined(AUDIO_IMA4_DECODER_HPP)
#define AUDIO_IMA4_DECODER_HPP

#include <memory>
#include <libGraphite/data/reader.hpp>
#include "core/audio/chunk.hpp"
#include "core/audio/audio_codec_descriptor.hpp"

namespace audio::ima4
{
    auto decode(const audio::codec_descriptor& descriptor, graphite::data::reader& r) -> std::shared_ptr<audio::chunk>;
}

#endif //IMA_4_HPP
