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

#include <libKestrel/exceptions/lua_runtime_exception.hpp>
#include <libKestrel/lua/script.hpp>
#include <libGraphite/data/reader.hpp>

#include <stdexcept>

// MARK: - Construction

kestrel::lua::script::script(const std::shared_ptr<runtime>& runtime, const resource::descriptor::lua_reference& ref)
    : m_runtime(runtime)
{
    if (!ref->has_id()) {
        throw lua_runtime_exception("No script id specified.");
    }

    if (auto script = ref->with_type(resource_type::code)->load()) {
        graphite::data::reader reader(&script->data());

        // Peek the first 4 bytes and determine if we're looking at raw Lua source code or byte code.
        auto lua_magic = reader.read_integer<std::uint32_t>() & 0xFFFFFF00;
        reader.set_position(0);

        if (lua_magic == 0x1B4C4A00) {
            // Bytecode
            m_bytecode = reader.data()->get<void *>();
            m_bytecode_size = reader.size();
            m_bytecode_offset = 0;
            m_format = format::bytecode;
        }
        else {
            // Source
            m_script = reader.read_cstr();
            m_format = format::source;
        }

        m_id = script->id();
        m_name = script->name();
    }
    else {
        throw lua_runtime_exception("Could not find/load lua script resource: " + ref->description());
    }
}

kestrel::lua::script::script(const std::shared_ptr<runtime>& runtime, const graphite::rsrc::resource *resource)
    : m_runtime(runtime)
{
    if (!resource) {
        throw lua_runtime_exception("No script specified");
    }

    graphite::data::reader reader(&resource->data());

    // Peek the first 4 bytes and determine if we're looking at raw Lua source code or byte code.
    auto lua_magic = reader.read_integer<std::uint32_t>() & 0xFFFFFF00;
    reader.set_position(0);

    if (lua_magic == 0x1B4C4A00) {
        // Bytecode
        m_bytecode = reader.data()->get<void *>();
        m_bytecode_size = reader.size() - 1;
        m_bytecode_offset = 0;
        m_format = format::bytecode;
    }
    else {
        // Source
        m_script = reader.read_cstr();
        m_format = format::source;
    }

    m_id = resource->id();
    m_name = resource->name();
}

kestrel::lua::script::script(const std::shared_ptr<runtime> &runtime, const std::string& code)
    : m_runtime(runtime), m_script(code), m_format(format::source), m_name("console.input"), m_id(-1)
{
}

// MARK: - Accessor

auto kestrel::lua::script::code() const -> std::string
{
    return m_script;
}

// MARK: - Execution

auto kestrel::lua::script::execute() const -> void
{
    if (m_script.empty() && m_format == format::source) {
        return;
    }
    else if (m_bytecode_size == 0 && m_format == format::bytecode) {
        return;
    }

    auto state = m_runtime.lock();
    if (!state) {
        throw lua_runtime_exception("Unable to execute script as the runtime is missing.");
    }
    state->run(*this);
}

auto kestrel::lua::script::id() const -> graphite::rsrc::resource::identifier
{
    return m_id;
}

auto kestrel::lua::script::name() const -> std::string
{
    return m_name;
}

auto kestrel::lua::script::bytecode() const -> void *
{
    return m_bytecode;
}

auto kestrel::lua::script::bytecode_size() const -> std::size_t
{
    return m_bytecode_size;
}

auto kestrel::lua::script::format() const -> enum format
{
    return m_format;
}

// MARK: - Chunk Reading

auto kestrel::lua::script::read_next_chunk() -> chunk_info
{
    struct chunk_info info {
        .size = m_bytecode_size,
        .ptr = (m_bytecode_offset >= m_bytecode_size) ? nullptr : m_bytecode
    };
    m_bytecode_offset = m_bytecode_size;
    return info;
}