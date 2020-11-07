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

#if !defined(KESTREL_RESOURCE_HPP)
#define KESTREL_RESOURCE_HPP

#include <string>
#include <optional>
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/asset/resource_reference.hpp"
#include <libGraphite/data/reader.hpp>

namespace asset
{

    struct resource: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<asset::resource> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        int64_t m_id;
        std::string m_name;
        std::string m_type;
        std::shared_ptr<graphite::data::reader> m_reader;

    public:
        resource(const std::string& type, const int64_t& id);
        lua_api explicit resource(const asset::resource_reference::lua_reference& ref);

        lua_api auto valid() const -> bool;

        lua_api auto id() const -> int64_t;
        lua_api auto name() const -> std::string;
        lua_api auto type() const -> std::string;

        lua_api auto read_signed_byte() -> int8_t;
        lua_api auto read_signed_short() -> int16_t;
        lua_api auto read_signed_long() -> int32_t;
        lua_api auto read_signed_quad() -> int64_t;
        lua_api auto read_byte() -> uint8_t;
        lua_api auto read_short() -> uint16_t;
        lua_api auto read_long() -> uint32_t;
        lua_api auto read_quad() -> uint64_t;
        lua_api auto read_pstr() -> std::string;
        lua_api auto read_cstr() -> std::string;
        lua_api auto read_cstr_width(const int& width) -> std::string;

        lua_api auto skip(const int& delta) -> void;
    };

}

#endif //KESTREL_RESOURCE_HPP
