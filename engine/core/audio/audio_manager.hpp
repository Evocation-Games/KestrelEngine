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

        auto play_background_audio(const std::string& path) -> void;
        auto stop_background_audio() -> void;

    };

}

#endif //AUDIO_MANAGER_HPP
