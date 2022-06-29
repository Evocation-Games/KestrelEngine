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

#include "core/asset/legacy/macintosh/item_list.hpp"
#include "core/environment.hpp"
#include "core/asset/cache.hpp"
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Lua

auto asset::legacy::macintosh::toolbox::item_list::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginNamespace("Toolbox")
                    .beginClass<item_list>("DialogItemList")
                        .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
                        .addStaticFunction("load", &item_list::load)
                        .addProperty("itemCount", &item_list::item_count)
                        .addFunction("itemAt", &item_list::at)
                    .endClass()
                    .beginClass<item_list::item>("DialogItem")
                        .addData("frame", &item::frame, false)
                        .addData("type", &item::type, false)
                        .addData("info", &item::info, false)
                    .endClass()
                .endNamespace()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::legacy::macintosh::toolbox::item_list::item_list(const asset::resource_descriptor::lua_reference &ref)
{
    // TODO: This should be moved to libGraphite
    if (auto resource = ref->with_type(type)->load()) {
        graphite::data::reader reader(&resource->data());

        m_item_count = reader.read_short();

        for (auto i = 0; i < m_item_count; ++i) {
            reader.move(4);

            struct item item;
            item.frame = math::rect::macintosh_rect(
                static_cast<double>(reader.read_signed_short()),
                static_cast<double>(reader.read_signed_short()),
                static_cast<double>(reader.read_signed_short()),
                static_cast<double>(reader.read_signed_short())
            );
            item.type = static_cast<item_type>(reader.read_byte());
            item.info = reader.read_pstr();

            if (reader.position() % 2 == 1) {
                reader.move();
            }

            m_items.emplace_back(std::move(item));
        }
    }
}

auto asset::legacy::macintosh::toolbox::item_list::load(const asset::resource_descriptor::lua_reference& ref) -> lua_reference
{
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(type, ref);
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }
    }

    auto ditl = lua_reference { new item_list(ref) };
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(type, ref, ditl);
    }
    return ditl;
}
