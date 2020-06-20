// Copyright (c) 2020 Tom Hancocks
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

#include <stdexcept>
#include "scripting/stack.hpp"
#include "scripting/state.hpp"

// MARK: - Construction

scripting::lua::stack::stack(std::weak_ptr<lua::state> state)
    : m_state(state)
{

}

// MARK: - Stack

auto scripting::lua::stack::pop(const int &count) -> void
{
    if (auto state = m_state.lock()) {
        lua_pop(state->internal_state(), count);
    }
    else {
        throw std::logic_error("lua::stack::pop used without parent state");
    }
}

auto scripting::lua::stack::peek_string(const scripting::lua::stack::index &i) -> std::string
{
    if (auto state = m_state.lock()) {
        return {lua_tostring(state->internal_state(), i)};
    }
    else {
        throw std::logic_error("lua::stack::peek_string used without parent state");
    }

}

auto scripting::lua::stack::pop_string() -> std::string
{
    if (auto state = m_state.lock()) {
        auto str = peek_string();
        pop();
        return str;
    }
    else {
        throw std::logic_error("lua::stack::pop_string used without parent state");
    }
}

