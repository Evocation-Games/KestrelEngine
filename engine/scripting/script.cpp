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

#include "scripting/script.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include <stdexcept>
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Construction

scripting::lua::script::script(const std::shared_ptr<lua::state>& state, const asset::resource_descriptor::lua_reference &ref)
    : m_state(state)
{
    if (!ref->has_id()) {
        throw std::runtime_error("No script id specified.");
    }

    if (auto script = ref->with_type(type)->load()) {
        m_name = script->name();
        m_id = script->id();
        graphite::data::reader reader(&script->data());
        m_object = new script_object();
        m_object->len = reader.size();
        m_object->data = reader.read_bytes(static_cast<int64_t>(m_object->len)).data();
    }
    else if (auto script = ref->with_type(script_type)->load()) {
        m_name = script->name();
        graphite::data::reader reader(&script->data());
        m_script = "-- " + ref->description() + "\n" + reader.read_cstr();
        m_id = script->id();
        m_name = script->name();
    }
    else {
        throw std::logic_error("Could not find/load lua script resource #" + std::to_string(ref->id));
    }
}

scripting::lua::script::script(const std::shared_ptr<lua::state>& state, const graphite::rsrc::resource *resource)
    : m_state(state)
{
    if (!resource) {
        throw std::runtime_error("No script specified.");
        return;
    }

    m_name = resource->name();
    graphite::data::reader reader(&resource->data());
    m_script = "-- " + std::to_string(resource->id()) + " - " + resource->name() + "\n" + reader.read_cstr();
    m_id = resource->id();
    m_name = resource->name();
}

scripting::lua::script::script(const std::shared_ptr<lua::state> &state, const std::string& code)
    : m_state(state), m_script(code), m_name("console.input"), m_id(-1)
{

}

// MARK: - Destruction

scripting::lua::script::~script()
{
    delete m_object;
}

// MARK: - Accessor

auto scripting::lua::script::code() const -> std::string
{
    return m_script;
}

auto scripting::lua::script::object() const -> void *
{
    return (m_object != nullptr) ? m_object : nullptr;
}

auto scripting::lua::script::object_size() const -> size_t
{
    return (m_object != nullptr) ? m_object->len : 0;
}

// MARK: - Execution

auto scripting::lua::script::execute() const -> void
{
    if (m_script.empty()) {
        return;
    }

    if (auto state = m_state.lock()) {
        state->run(*this);
    }
}

auto scripting::lua::script::id() const -> int64_t
{
    return m_id;
}

auto scripting::lua::script::name() const -> std::string
{
    return m_name;
}

auto scripting::lua::script::is_object() const -> bool
{
    return (m_object != nullptr);
}
