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
#include <libKestrel/ui/scene/interface.hpp>
#include <libKestrel/cache/cache.hpp>
#include <libKestrel/kestrel.hpp>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Construction

kestrel::ui::scene_interface::scene_interface(const resource::descriptor::lua_reference &ref)
{
    if (auto resource = ref->with_type(resource_type::code)->load()) {
        graphite::data::reader reader(&resource->data());

        m_flags = static_cast<enum scene_interface_flags>(reader.read_short());
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

auto kestrel::ui::scene_interface::parse_item_list(graphite::data::reader &reader, std::uint32_t count) -> std::vector<scene_interface_item::lua_reference>
{
    std::vector<scene_interface_item::lua_reference> items;

    for (auto n = 0; n < count && !reader.eof(); ++n) {
        scene_interface_item::lua_reference item(new scene_interface_item());
        item->set_type(static_cast<enum ui::control_type>(reader.read_byte()));
        item->set_identifier(reader.read_pstr());
        item->set_frame(math::rect(
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short())
        ));
        item->set_value(reader.read_cstr());
        item->set_lua_script(reader.read_cstr());

        auto item_count = reader.read_long();
        if (item_count > 0) {
            item->set_children(std::move(parse_item_list(reader, item_count)));
        }

        items.emplace_back(std::move(item));
    }

    return std::move(items);
}
