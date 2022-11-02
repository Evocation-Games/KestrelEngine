// Copyright (c) 2022 Tom Hancocks
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

#pragma once

#include <memory>
#include <libKestrel/event/event.hpp>

namespace kestrel
{
    struct responder_chain
    {
    public:
        struct responder : public std::enable_shared_from_this<responder>
        {
        public:
            auto attach_responder_chain(responder_chain *chain) -> void;

            auto become_first_responder() -> bool;
            auto resign_first_responder() -> bool;

            virtual auto did_become_first_responder() -> void;
            virtual auto did_resign_first_responder() -> void;

            virtual auto receive_event(const event& e) -> bool;

        private:
            responder_chain *m_chain { nullptr };
            responder_chain::responder *m_next_responder { nullptr };
        };

        struct mouse_responder : public std::enable_shared_from_this<mouse_responder>
        {
        public:
            virtual auto receive_event(const event& e) -> bool = 0;
        };

        struct key_responder : public std::enable_shared_from_this<mouse_responder>
        {
        public:
            virtual auto receive_event(const event& e) -> bool = 0;
        };

    public:
        responder_chain() = default;

        auto current_responder() -> responder *;

        auto set_initial_responder(responder *responder) -> void;
        auto set_first_responder(responder *responder) -> bool;

        auto add_mouse_responder(mouse_responder *responder) -> void;

        auto send_event(const event& e) -> bool;

    private:
        responder *m_first_responder { nullptr };
        responder *m_initial_responder { nullptr };
        std::vector<mouse_responder *> m_mouse_responders;
    };
}
