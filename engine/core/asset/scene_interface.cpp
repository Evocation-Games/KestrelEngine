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

#include <stdexcept>
#include "core/asset/scene_interface.hpp"
#include "core/environment.hpp"
#include "core/asset/cache.hpp"
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua

auto asset::scene_interface::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<scene_interface>("SceneInterface")
            .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
            .addProperty("name", &scene_interface::name)
            .addProperty("flags", &scene_interface::flags)
            .addProperty("size", &scene_interface::scene_size)
            .addProperty("useImGui", &scene_interface::uses_imgui)
            .addProperty("showsTitle", &scene_interface::shows_title_in_imgui)
            .addProperty("showsCloseButton", &scene_interface::shows_close_button_in_imgui)
            .addProperty("hasPassthrough", &scene_interface::has_scene_passthrough)
            .addProperty("itemCount", &scene_interface::child_count)
            .addFunction("itemAt", &scene_interface::child_at)
        .endClass();
}

// MARK: - Construction

asset::scene_interface::scene_interface(const asset::resource_descriptor::lua_reference &ref)
{
    if (auto resource = ref->with_type(type)->load()) {
        graphite::data::reader reader(&resource->data());

        m_flags = static_cast<enum flags>(reader.read_short());
        m_name = reader.read_pstr();
        m_scene_size = math::size(
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short())
        );

        auto item_count = reader.read_short();
        m_items = std::move(parse_item_list(reader, item_count));
        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto asset::scene_interface::parse_item_list(graphite::data::reader &reader, uint32_t count) -> std::vector<struct item>
{
    std::vector<struct item> items;

    for (auto n = 0; n < count && !reader.eof(); ++n) {
        struct item item;
        item.set_type(static_cast<enum ui::control_definition::type>(reader.read_byte()));
        item.set_identifier(reader.read_pstr());
        item.set_frame(math::rect(
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short())
        ));
        item.set_value(reader.read_cstr());
        item.set_lua_script(reader.read_cstr());

        auto item_count = reader.read_long();
        if (item_count > 0) {
            item.set_children(std::move(parse_item_list(reader, item_count)));
        }

        items.emplace_back(std::move(item));
    }

    return std::move(items);
}
