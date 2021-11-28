//
// Created by Tom Hancocks on 13/11/2021.
//

#if !defined(AUDIO_MANAGER_HPP)
#define AUDIO_MANAGER_HPP

#include <vector>
#include <functional>
#include <libGraphite/data/reader.hpp>
#include "core/audio/sound.hpp"
#include "core/audio/chunk.hpp"
#include "core/audio/audio_codec_descriptor.hpp"

namespace audio
{

    class manager
    {
    public:
        struct playback_reference
        {
            uint64_t id;
            std::shared_ptr<audio::chunk> chunk;
            std::function<auto()->void> completion_callback;
            bool finished;

            playback_reference(uint64_t id, std::shared_ptr<audio::chunk> chunk, std::function<auto()->void> completion_callback)
                : id(id), chunk(std::move(chunk)), completion_callback(std::move(completion_callback)), finished(false) {};
        };

    private:
        manager() = default;
        uint64_t m_next_ref_id { 1 };
        std::vector<playback_reference> m_playback_refs;

        auto construct_playback_reference(std::shared_ptr<audio::chunk> chunk, std::function<auto()->void> completion) -> playback_reference *;

    public:
        manager(const manager&) = delete;
        manager& operator=(const manager &) = delete;
        manager(manager &&) = delete;
        manager & operator=(manager &&) = delete;

        static auto shared_manager() -> manager&;

        auto configure() -> void;
        auto monitor_finished_playbacks() -> void;

        auto play(std::shared_ptr<audio::chunk> chunk, std::function<auto()->void> completion) -> void;
        auto finish_playback(uint64_t playback_id) -> void;

    };

}

#endif //AUDIO_MANAGER_HPP
