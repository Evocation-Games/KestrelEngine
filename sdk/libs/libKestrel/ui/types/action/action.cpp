// Copyright (c) 2023 Tom Hancocks
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

#include <libKestrel/ui/types/action/action.hpp>
#include <libKestrel/ui/scene/game_scene.hpp>
#include <libKestrel/kestrel.hpp>

// MARK: - Construction

kestrel::ui::action::action(const ::ui::format::action &action)
{
    switch (action.type()) {
        case ::ui::format::action::type::push_named_scene: {
            m_type = type::push;
            m_value = action.value<std::string>();
            break;
        }
        case ::ui::format::action::type::push_identified_scene: {
            m_type = type::push;
            m_value = resource::descriptor::lua_reference(new resource::descriptor(action.value<::resource::reference>()));
            break;
        }
        case ::ui::format::action::type::named_function: {
            m_type = type::function;
            m_value = action.value<std::string>();
            break;
        }
        case ::ui::format::action::type::lua_script_reference: {
            resource::descriptor::lua_reference ref(new resource::descriptor(action.value<::resource::reference>()));
            m_type = type::lua;
            m_value = lua::script(kestrel::lua_runtime(), ref);
            break;
        }
        case ::ui::format::action::type::lua_script_source: {
            m_type = type::lua;
            m_value = lua::script(kestrel::lua_runtime(), action.value<std::string>());
            break;
        }
        case ::ui::format::action::type::lua_script_bytecode: {
            m_type = type::lua;
            m_value = lua::script(kestrel::lua_runtime(), action.value<data::block>());
            break;
        }
        default:
            break;
    }
}

// MARK: - Scene Management

auto kestrel::ui::action::bind_to_scene(const ui::game_scene *scene) -> action&
{
    m_scene = scene;
    return *this;
}

// MARK: - Execution

auto kestrel::ui::action::execute() -> void
{
    switch (m_type) {
        case type::push: {
            if (m_value.index() == 0) {
                const auto& name = std::get<std::string>(m_value);
                kestrel::scene_definition(name)->push();
            }
            else if (m_value.index() == 2) {
                const auto& ref = std::get<resource::descriptor::lua_reference>(m_value);
                kestrel::scene_definition(ref)->push();
            }
            break;
        }
        case type::function: {
            if (m_value.index() == 0) {
                const auto& function = m_scene->find_function(std::get<std::string>(m_value));
                if (function.state() && function.isFunction()) {
                    function();
                }
            }
            break;
        }
        case type::lua: {
            if (m_value.index() == 0) {
                lua::script script(kestrel::lua_runtime(), std::get<std::string>(m_value));
                script.execute();
            }
            else if (m_value.index() == 1) {
                std::get<lua::script>(m_value).execute();
            }
            else if (m_value.index() == 2) {
                const auto& ref = std::get<resource::descriptor::lua_reference>(m_value);
                if (ref.get()) {
                    lua::script script(kestrel::lua_runtime(), ref);
                    script.execute();
                }
            }
            break;
        }
        default:
            break;
    }
}
