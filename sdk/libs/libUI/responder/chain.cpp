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

#include <libUI/responder/chain.hpp>
#include <libUI/responder/responder.hpp>

// MARK: - Responder Management

auto ui::responder_chain::current_responder() -> responder *
{
    return m_nodes.first;
}

auto ui::responder_chain::set_initial_responder(ui::responder *responder) -> void
{
    // TODO: Verify responder is present in the chain
    m_nodes.initial = responder;
}

auto ui::responder_chain::set_first_responder(ui::responder *responder) -> bool
{
    if (auto previous = m_nodes.first) {
        previous->did_resign_first_responder();
    }

    m_nodes.first = responder;
    if (responder) {
        responder->did_become_first_responder();
    }

    return true;
}

auto ui::responder_chain::add_responder(ui::responder *responder) -> void
{
    if (!responder) {
        return;
    }

    responder->m_chain = this;
    if (!m_nodes.start) {
        m_nodes.initial = m_nodes.first = m_nodes.start = m_nodes.end = responder;
    }
    else {
        responder->m_next = m_nodes.start;
        m_nodes.end->m_next = responder;
        m_nodes.end = responder;
    }
}

auto ui::responder_chain::remove_responder(ui::responder *responder) -> void
{
    auto previous = m_nodes.end;
    auto current = m_nodes.start;
    while (current && previous) {
        if (current == responder) {
            if (current == m_nodes.end) {
                previous->m_next = m_nodes.start;
                m_nodes.end = previous;
            }
            else if (current == m_nodes.start) {
                m_nodes.start = current->m_next;
                m_nodes.end->m_next = m_nodes.end->m_next;
            }
            else {
                previous->m_next = current->m_next;
            }

            if (m_nodes.first == responder) {
                m_nodes.first = previous;
            }
            if (m_nodes.initial == responder) {
                m_nodes.initial = m_nodes.start;
            }
            return;
        }
        previous = current;
        current = current->next_responder();
    }
}

auto ui::responder_chain::has_responder(ui::responder *responder) -> bool
{
    auto visitor = m_nodes.start;
    while (visitor) {
        if (visitor == responder) {
            return true;
        }
        visitor = visitor->next_responder();
    }
    return false;
}

auto ui::responder_chain::next_responder() -> void
{
    if (m_nodes.first) {
        m_nodes.first = m_nodes.first->next_responder();
    }
    else {
        m_nodes.first = m_nodes.initial ?: m_nodes.start;
    }
}

// MARK: - Events

auto ui::responder_chain::send_event(const ui::event &event) -> bool
{
    if (event.is_key_event()) {
        if (auto first = m_nodes.first) {
            return first->send_event(event);
        }
    }

    if (event.is_mouse_event()) {
        auto responder = m_nodes.start;
        while (responder) {
            if (responder->send_event(event)) {
                return true;
            }
            responder = responder->next_responder();
        }
    }

    return false;
}
