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

#pragma once

#include <optional>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libGraphite/rsrc/resource.hpp>

namespace kestrel::resource
{
    struct lua_api(Resource.Key, Available_0_8) key
    {
    public:
        has_constructable_lua_api(key);

        explicit key(const descriptor& descriptor);

        lua_getter(toString, Available_0_8) [[nodiscard]] auto str() const -> std::string;
        lua_getter(hash, Available_0_8) [[nodiscard]] auto hash() const -> std::string;

        auto operator==(const key& rhs) const -> bool;

    private:
        std::optional<graphite::rsrc::resource::identifier> m_id;
        std::optional<std::string> m_type;
        std::optional<std::string> m_container;
    };
}