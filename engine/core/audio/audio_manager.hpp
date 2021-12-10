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

#include <memory>
#include <vector>
#include <complex>
#include <functional>

#include "core/audio/player/core_audio_player.hpp"
#include "core/audio/player/openal_player.hpp"
#include "core/audio/player/player_item.hpp"

namespace audio
{

    class manager
    {
    public:
        enum class library { none, core_audio, openal };

    private:
        std::shared_ptr<audio::core_audio::player> m_core_audio;
        std::shared_ptr<audio::openal::player> m_openal;
        library m_library { library::none };

        manager() = default;

    public:
        manager(const manager&) = delete;
        manager& operator=(const manager &) = delete;
        manager(manager &&) = delete;
        manager & operator=(manager &&) = delete;

        static auto shared_manager() -> manager&;

        auto set_library(library lib) -> void;
        [[nodiscard]] auto current_library() const -> library;

        auto play_item(std::shared_ptr<player_item> item, std::function<auto()->void> completion) -> playback_session_ref;
        auto stop_item(const playback_session_ref& ref) -> void;

        auto finish_item(const playback_session_ref& ref) -> void;
    };

}

#endif //AUDIO_MANAGER_HPP
