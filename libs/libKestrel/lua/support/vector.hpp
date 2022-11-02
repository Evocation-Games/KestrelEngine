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
    struct lua_api(Vector, Available_0_8) vector
    {
        lua_declare_named(ResourceSet, resource::descriptor::lua_reference)
        lua_declare_named(StringVector, std::string)
        lua_declare_named(DirectoryContentsVector, sandbox::file_reference::lua_reference)
        lua_declare_named(ModList, sandbox::mod_reference::lua_reference)
        has_named_constructable_lua_api(vector<T>);

        vector() = default;
        explicit vector(const std::vector<T>& v) : m_items(v) {}
        explicit vector(const T& item, std::size_t count = 1) : m_items(count, item) {}

        lua_function(each, Available_0_8) auto each(const luabridge::LuaRef& body) const -> void
        {
            for (const auto& item : m_items) {
                body(item);
            }
        }

        lua_getter(isEmpty, Available_0_8) [[nodiscard]] auto empty() const -> bool
        {
            return size() == 0;
        }

        lua_getter(count, Available_0_8) [[nodiscard]] auto size() const -> int
        {
            return m_items.size();
        }

        lua_function(at, Available_0_8) [[nodiscard]] auto lua_at(const int& i) const -> T
        {
            return at(i - 1);
        }

        lua_function(clear, Available_0_8) auto clear() -> void
        {
            m_items.clear();
        }

        [[nodiscard]] auto at(const int& i) const -> T
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

        auto operator[](std::size_t idx) const -> T
        {
            return m_items[idx];
        }

        auto replace(std::size_t idx, T item) -> void
        {
            m_items[idx] = item;
        }

        auto front() -> T
        {
            return m_items.front();
        }

        auto back() -> T
        {
            return m_items.back();
        }

        auto begin() -> typename std::vector<T>::iterator
        {
            return m_items.begin();
        }

        auto end() -> typename std::vector<T>::iterator
        {
            return m_items.end();
        }

        [[nodiscard]] auto begin() const -> typename std::vector<T>::const_iterator
        {
            return m_items.begin();
        }

        [[nodiscard]] auto end() const-> typename std::vector<T>::const_iterator
        {
            return m_items.end();
        }

        [[nodiscard]] auto to_vector() const -> std::vector<T>
        {
            return m_items;
        }

    private:
        std::vector<T> m_items;
    };
}
