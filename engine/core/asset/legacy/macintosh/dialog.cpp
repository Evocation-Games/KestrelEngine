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

#include "core/asset/legacy/macintosh/dialog.hpp"
#include "core/environment.hpp"
#include "core/asset/cache.hpp"
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Lua

auto asset::legacy::macintosh::toolbox::dialog::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginNamespace("Toolbox")
                    .beginClass<dialog>("Dialog")
                        .addConstructor<auto(*)(const asset::resource_descriptor::lua_reference&)->void, lua_reference>()
                        .addStaticFunction("load", &dialog::load)
                        .addProperty("bounds", &dialog::bounds)
                        .addProperty("procId", &dialog::proc_id)
                        .addProperty("visible", &dialog::visible)
                        .addProperty("goAway", &dialog::go_away)
                        .addProperty("refCon", &dialog::ref_con)
                        .addProperty("interfaceList", &dialog::interface_list)
                        .addProperty("title", &dialog::title)
                        .addProperty("autoPosition", &dialog::auto_position)
                    .endClass()
                .endNamespace()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::legacy::macintosh::toolbox::dialog::dialog(const asset::resource_descriptor::lua_reference& ref)
{
    // TODO: This should be moved to libGraphite
    if (auto resource = ref->with_type(type)->load()) {
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

auto asset::legacy::macintosh::toolbox::dialog::load(const asset::resource_descriptor::lua_reference& ref) -> lua_reference
{
    if (auto env = environment::active_environment().lock()) {
        auto asset = env->cache()->fetch(type, ref);
        if (asset.has_value()) {
            return std::any_cast<lua_reference>(asset.value());
        }
    }

    auto dlog = lua_reference { new dialog(ref) };
    if (auto env = environment::active_environment().lock()) {
        env->cache()->add(type, ref, dlog);
    }
    return dlog;
}