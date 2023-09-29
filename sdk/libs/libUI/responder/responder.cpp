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

#include <libUI/responder/responder.hpp>
#include <libUI/responder/chain.hpp>

// MARK: - Responder

auto ui::responder::attach_to_responder_chain(ui::responder_chain *chain) -> void
{
    m_chain = chain;
}

auto ui::responder::become_first_responder() -> bool
{
    return m_chain && m_chain->set_first_responder(this);
}

auto ui::responder::resign_first_responder() -> bool
{
    return m_chain && m_chain->set_first_responder(m_next);
}

auto ui::responder::next_responder() const -> responder *
{
    return m_next;
}

// MARK: - Notifications

auto ui::responder::did_become_first_responder() -> void
{}

auto ui::responder::did_resign_first_responder() -> void
{}

// MARK: - Events

auto ui::responder::send_event(const ui::event &event) -> bool
{
    return false;
}