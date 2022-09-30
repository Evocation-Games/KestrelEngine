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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::lua
{
    /**
     * A vector that is compatible with the Lua Scripting Environment, aware of the method of indexing
     * that Lua uses.
     * @tparam T    The type of value contained in the vector.
     */
    template<typename T>
    lua_api(0.8) struct vector
    {
        has_named_constructable_lua_api(vector<T>) {
            runtime->global_namespace()
                .beginClass<vector<T>>(name.c_str())
                    .addProperty("count", &vector<T>::size)
                    .addProperty("isEmpty", &vector<T>::empty)
                    .addFunction("at", &vector<T>::lua_at)
                    .addFunction("each", &vector<T>::each)
                .endClass();
        };

        vector() = default;
        explicit vector(const std::vector<T>& v) : m_items(v) {}

        lua_api(0.8) auto each(const luabridge::LuaRef& body) const -> void
        {
            for (const auto& item : m_items) {
                body(item);
            }
        }

        lua_api(0.8) [[nodiscard]] auto empty() const -> bool
        {
            return size() == 0;
        }

        lua_api(0.8) [[nodiscard]] auto size() const -> int
        {
            return m_items.size();
        }

        lua_api(0.8) auto lua_at(const int& i) const -> T
        {
            return at(i - 1);
        }

        auto at(const int& i) const -> T
        {
            return m_items.at(i);
        }

        auto reserve(const int& count) -> void
        {
            m_items.reserve(count);
        }

        auto emplace_back(const T& r) -> void
        {
            m_items.emplace_back(r);
        }

        auto append(const vector<T>& v) -> void
        {
            m_items.insert(m_items.end(), v.m_items.begin(), v.m_items.end());
        }

        auto sort(std::function<auto(const T&, const T&)->bool> fn) -> void
        {
            std::sort(m_items.begin(), m_items.end(), fn);
        }

    private:
        std::vector<T> m_items;
    };
}
