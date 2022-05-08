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

#include <vector>
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "scripting/state.hpp"
#include "math/rect.hpp"
#include "util/hint.hpp"

namespace asset::legacy::macintosh::toolbox
{
    struct item_list : public scripting::lua::object
    {
    public:
        enum class item_type : uint8_t
        {
            user_item = 0,
            help_item = 1,
            button = 4,
            checkbox = 5,
            radio = 6,
            control = 7,
            static_text = 8,
            edit_text = 16,
            icon = 32,
            picture = 64,
            disable = 128,
        };

        struct item
        {
            math::rect frame { 0, 0, 0, 0 };
            item_type type { item_type::user_item };
            std::string info { "" };
        };

    public:
        constexpr static const char *type { "DITL" };
        typedef luabridge::RefCountedPtr<item_list> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit item_list(const asset::resource_descriptor::lua_reference& ref);
        lua_api static auto load(const asset::resource_descriptor::lua_reference& ref) -> lua_reference;

        [[nodiscard]] lua_api auto item_count() const -> uint16_t { return m_item_count; }
        [[nodiscard]] lua_api auto at(uint16_t idx) const -> const item& { return m_items[idx]; }

    private:
        uint16_t m_item_count { 0 };
        std::vector<item> m_items;
    };
}

