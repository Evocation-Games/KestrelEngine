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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <memory>
#include <functional>
#include "core/audio/player/player_item.hpp"

namespace audio
{
    typedef uint64_t playback_session_ref;

    template<typename player_info>
    struct playback_session
    {
        playback_session_ref handle;
        std::shared_ptr<audio::player_item> item;
        std::function<auto()->void> playback_finished;
        player_info info;

        playback_session(uint64_t handle, std::shared_ptr<audio::player_item> item, player_info info, std::function<auto()->void> playback_finished)
            : handle(handle), item(std::move(item)), info(std::move(info)), playback_finished(std::move(playback_finished)) {}
    };

    template<typename player_info>
    class player
    {
    private:
        playback_session_ref m_next_handle { 0 };
        std::vector<audio::playback_session<player_info>> m_playback_sessions;

    public:
        player() = default;

        auto play(std::shared_ptr<audio::player_item> item, std::function<auto()->void> finished) -> playback_session_ref;
        auto stop(const playback_session_ref& ref, bool no_finish = true) -> void;

    };

}


#endif //PLAYER_HPP
