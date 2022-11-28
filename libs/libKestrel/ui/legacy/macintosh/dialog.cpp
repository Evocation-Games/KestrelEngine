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

#include <libKestrel/ui/legacy/macintosh/dialog.hpp>
#include <libKestrel/cache/cache.hpp>
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Construction

kestrel::ui::legacy::macintosh::toolbox::dialog::dialog(const resource::descriptor::lua_reference& ref)
{
    // TODO: This should be moved to libGraphite
    if (auto resource = ref->with_type(resource_type::code)->load()) {
        graphite::data::reader reader(&resource->data());

        m_bounds = math::rect::macintosh_rect(
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short()),
            static_cast<double>(reader.read_signed_short())
        );

        m_proc_id = reader.read_signed_short();
        m_visible = reader.read_short() != 0;
        m_go_away = reader.read_short() != 0;
        m_ref_con = reader.read_signed_long();
        m_interface_list = reader.read_signed_short();
        m_title = reader.read_pstr();

        if (reader.position() % 2 != 0) {
            reader.move();
        }

        m_auto_position = reader.read_short();
        return;
    }
    throw std::logic_error("Bad resource reference encountered: Unable to load resource.");
}

auto kestrel::ui::legacy::macintosh::toolbox::dialog::load(const resource::descriptor::lua_reference& ref) -> lua_reference
{
    auto cached_asset = cache::fetch(ref->with_type(resource_type::code));
    if (cached_asset.has_value()) {
        return std::any_cast<lua_reference>(cached_asset.value());
    }

    auto dlog = lua_reference { new dialog(ref) };
    cache::add(ref->with_type(resource_type::code), dlog);
    return dlog;
}