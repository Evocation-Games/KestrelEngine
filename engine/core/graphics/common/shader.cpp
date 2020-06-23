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
#include "core/graphics/common/shader.hpp"
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Construction

graphics::shader::shader(const std::string &type, const int64_t &vertex_id, const int64_t& fragment_id)
{
    if (auto res = graphite::rsrc::manager::shared_manager().find(type, vertex_id).lock()) {
        m_vertex_name = res->name();
        graphite::data::reader r(res->data());
        m_vertex_code = r.read_cstr();
    }
    else {
        throw std::logic_error("Failed to load vertex shader '" + type + "' #" + std::to_string(vertex_id));
    }

    if (auto res = graphite::rsrc::manager::shared_manager().find(type, fragment_id).lock()) {
        m_fragment_name = res->name();
        graphite::data::reader r(res->data());
        m_fragment_code = r.read_cstr();
    }
    else {
        throw std::logic_error("Failed to load fragment shader '" + type + "' #" + std::to_string(fragment_id));
    }
}

// MARK: - Accessors

auto graphics::shader::vertex_name() const -> std::string
{
    return m_vertex_name;
}

auto graphics::shader::vertex_code() const -> std::string
{
    return m_vertex_code;
}

auto graphics::shader::fragment_name() const -> std::string
{
    return m_fragment_name;
}

auto graphics::shader::fragment_code() const -> std::string
{
    return m_fragment_code;
}

// MARK: - Shader Interaction

auto graphics::shader::use() -> std::shared_ptr<graphics::shader>
{
    return shared_from_this();
}
