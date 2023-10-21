// Copyright (c) 2023 Tom Hancocks
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

#include <libUI/event/event.hpp>

namespace ui
{
    struct responder;

    struct responder_chain
    {
    public:
        responder_chain() = default;

        auto current_responder() -> responder *;

        auto add_responder(responder *responder) -> void;
        auto remove_responder(responder *responder) -> void;
        auto has_responder(responder *responder) -> bool;
        auto next_responder() -> void;

        auto set_initial_responder(responder *responder) -> void;
        auto set_first_responder(responder *responder) -> bool;

        auto send_event(const event& event) -> bool;

    private:
        struct {
            responder *first { nullptr };
            responder *initial { nullptr };
            responder *start { nullptr };
            responder *end { nullptr };
        } m_nodes;
    };
}