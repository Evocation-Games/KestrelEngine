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

#include "scripting/lua/lua_state.hpp"
#include "scripting/lua/lua_script.hpp"
#include "libGraphite/rsrc/manager.hpp"
#include "libGraphite/data/reader.hpp"

// MARK: - Construction

kestrel::lua::script::script()
{

}

kestrel::lua::script::script(int64_t id)
    : m_id(id)
{
    if (auto s = graphite::rsrc::manager::shared_manager().find(type, id).lock()) {
        m_name = s->name();
        graphite::data::reader r(s->data());
        m_script = r.read_cstr();
    }
}

// MARK: - Execution

auto kestrel::lua::script::execute() const -> void
{
    if (!m_script.empty()) {
        lua::state::global().load_script(m_script);
    }
}