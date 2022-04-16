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

#pragma once

#include <vector>
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace util
{

    template<typename T>
    class lua_vector: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<util::lua_vector<T>> lua_reference;

    private:
        std::vector<T> m_items;

    public:
        static auto enroll_object_api_in_state(const std::string &name, const std::shared_ptr<scripting::lua::state>& lua) -> void
        {
            luabridge::getGlobalNamespace(lua->internal_state())
                .beginClass<util::lua_vector<T>>(name.c_str())
                    .addProperty("count", &util::lua_vector<T>::size)
                    .addProperty("isEmpty", &util::lua_vector<T>::empty)
                    .addFunction("at", &util::lua_vector<T>::lua_at)
                    .addFunction("each", &util::lua_vector<T>::each)
                .endClass();
        }

        lua_vector() = default;
        explicit lua_vector(const std::vector<T>& v) : m_items(v) {}

        lua_api auto each(const luabridge::LuaRef& body) const -> void
        {
            for (const auto& item : m_items) {
                body(item);
            }
        }

        lua_api [[nodiscard]] auto empty() const -> bool
        {
            return size() == 0;
        }

        lua_api [[nodiscard]] auto size() const -> int
        {
            return m_items.size();
        }

        lua_api auto lua_at(const int& i) const -> T
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

        auto sort(std::function<auto(const T&, const T&)->bool> fn) -> void
        {
            std::sort(m_items.begin(), m_items.end(), fn);
        }

    };

}
