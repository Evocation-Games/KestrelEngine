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
#include "scripting/lua/lua.hpp"
#include "foundation/assets/resource_reference.hpp"
#include "libGraphite/data/reader.hpp"

namespace kestrel { namespace assets {

    struct resource : public lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<assets::resource> lua_reference;

    private:
        int64_t m_id;
        std::string m_name;
        std::string m_type;
        std::optional<graphite::data::reader> m_resource_reader;

    public:
        static auto register_object() -> void;

        resource(resource_reference::lua_reference ref);

        auto valid() const -> bool;
        auto id() const -> int64_t;
        auto name() const -> std::string;

        auto read_signed_byte() -> int8_t;
        auto read_signed_short() -> int16_t;
        auto read_signed_long() -> int32_t;
        auto read_signed_quad() -> int64_t;
        auto read_byte() -> uint8_t;
        auto read_short() -> uint16_t;
        auto read_long() -> uint32_t;
        auto read_quad() -> uint64_t;
        auto read_pstr() -> std::string;
        auto read_cstr() -> std::string;
        auto read_cstr_width(int width) -> std::string;
    };

}};

#endif //KESTREL_RESOURCE_HPP
