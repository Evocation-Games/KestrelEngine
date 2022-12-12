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

#include <libKestrel/event/responder/responder_chain.hpp>

// MARK: - Current Responder

auto kestrel::responder_chain::responder::attach_responder_chain(responder_chain *chain) -> void
{
    m_chain = chain;
}

auto kestrel::responder_chain::responder::become_first_responder() -> bool
{
    return m_chain && m_chain->set_first_responder(this);
}

auto kestrel::responder_chain::responder::resign_first_responder() -> bool
{
    return m_chain && m_chain->set_first_responder(m_next_responder);
}

auto kestrel::responder_chain::responder::did_become_first_responder() -> void
{
}

auto kestrel::responder_chain::responder::did_resign_first_responder() -> void
{
}

auto kestrel::responder_chain::responder::receive_event(const event &e) -> bool
{
    return false;
}

// MARK: - Responder Chain

auto kestrel::responder_chain::current_responder() -> responder *
{
    return m_first_responder;
}

auto kestrel::responder_chain::set_initial_responder(responder *responder) -> void
{
    m_initial_responder = responder;
}

auto kestrel::responder_chain::set_first_responder(responder *responder) -> bool
{
    if (auto last = m_first_responder) {
        last->did_resign_first_responder();
    }

    m_first_responder = responder;
    if (responder) {
        responder->did_become_first_responder();
    }

    return true;
}

auto kestrel::responder_chain::add_mouse_responder(mouse_responder *responder) -> void
{
    m_mouse_responders.emplace_back(responder);
}

auto kestrel::responder_chain::send_event(const event &e) -> bool
{
    if (e.has(event_type::any_mouse_down) || e.has(event_type::any_mouse_up)) {
        for (auto receiver: m_mouse_responders) {
            if (receiver->receive_event(e)) {
                return true;
            }
        }
    }

    if (e.is_key_event()) {
        if (auto receiver = m_first_responder) {
            return receiver->receive_event(e);
        }
    }

    return false;
}
