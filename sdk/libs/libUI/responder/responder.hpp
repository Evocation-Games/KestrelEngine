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
    struct responder_chain;

    struct responder
    {
    public:
        auto attach_to_responder_chain(responder_chain *chain) -> void;

        auto next_responder() const -> responder *;

        auto become_first_responder() -> bool;
        auto resign_first_responder() -> bool;

        virtual auto did_become_first_responder() -> void;
        virtual auto did_resign_first_responder() -> void;

        virtual auto send_event(const event& event) -> bool;

        friend struct responder_chain;

    private:
        responder_chain *m_chain { nullptr };
        responder *m_next { nullptr };
    };
}