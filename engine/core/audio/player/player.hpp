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

#if! defined(PLAYER_HPP)
#define PLAYER_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>
#include "core/audio/player/player_item.hpp"

namespace audio
{
    typedef uint64_t playback_session_ref;

    template<typename player_info>
    struct playback_session
    {
        playback_session_ref ref;
        std::shared_ptr<audio::player_item> item;
        std::function<auto()->void> playback_finished;
        player_info info;

        playback_session(playback_session_ref ref, std::shared_ptr<audio::player_item> item, player_info info, std::function<auto()->void> playback_finished)
            : ref(ref), item(std::move(item)), info(std::move(info)), playback_finished(std::move(playback_finished)) {}

        auto finish() -> void { playback_finished(); }
    };

    template<typename player_info>
    class player
    {
    private:
        playback_session_ref m_cur_ref { 0 };
        std::vector<std::shared_ptr<audio::playback_session<player_info>>> m_playback_sessions;

        auto next_session_ref() -> playback_session_ref
        {
            return (++m_cur_ref);
        }

    public:
        player() = default;

        virtual auto play(std::shared_ptr<audio::player_item> item, std::function<auto()->void> finished) -> playback_session_ref
        {
            auto session = std::make_shared<audio::playback_session<player_info>>(
                    next_session_ref(), std::move(item), std::move(acquire_player_info()), std::move(finished)
            );

            configure_playback_session(session);
            m_playback_sessions.template emplace_back(session);

            return session->ref;
        }

        virtual auto stop(const playback_session_ref& ref) -> void
        {

        }

        virtual auto configure() -> void {}

        virtual auto acquire_player_info() -> player_info
        {
            throw std::runtime_error("This method must be implemented by a subclass.");
        }

        virtual auto configure_playback_session(std::shared_ptr<audio::playback_session<player_info>> session) -> void
        {
            throw std::runtime_error("This method must be implemented by a subclass.");
        }

        virtual auto playback_session(playback_session_ref ref) const -> std::shared_ptr<audio::playback_session<player_info>>
        {
            for (const auto& session : m_playback_sessions) {
                if (session->ref == ref) {
                    return session;
                }
            }
            return nullptr;
        }
    };

}

#endif //PLAYER_HPP
