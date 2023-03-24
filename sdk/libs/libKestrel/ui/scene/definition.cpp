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

#include <libKestrel/ui/scene/definition.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/resource/reader.hpp>
#include <libKestrel/ui/dialog/dialog_configuration.hpp>

// MARK: - Construction

kestrel::ui::scene_definition::scene_definition(const std::string &name)
    : m_name(name)
{
    // Get the container that the scene exists within.
    auto container = kestrel::get_scene_container(name);
    if (!container.get()) {
        // The scene doesn't exist.
        // TODO: Log a warning here.
        return;
    }

    // Now lookup the definition.
    m_descriptor = container->typed_identified_resource(resource_type::code, constants::definition_id);
    if (!m_descriptor.get() || !m_descriptor->valid()) {
        // Still could not find the scene...
        // TODO: Log a warning here.
        return;
    }

    // Attempt to load the scene definition data.
    load();
}

// MARK: - Accessors

auto kestrel::ui::scene_definition::name() const -> std::string
{
    return m_name;
}

auto kestrel::ui::scene_definition::valid() const -> bool
{
    return m_valid;
}

auto kestrel::ui::scene_definition::should_use_macintosh_dialog() const -> bool
{
    return (m_dialog.get() && !(m_interface.get() && m_interface->valid()));
}

auto kestrel::ui::scene_definition::scene_interface() const -> scene_interface::lua_reference
{
    return { nullptr };
}

auto kestrel::ui::scene_definition::macintosh_dialog() const -> ui::legacy::macintosh::toolbox::dialog::lua_reference
{
    return { nullptr };
}

auto kestrel::ui::scene_definition::main_script() const -> lua::script
{
    return { kestrel::lua_runtime(), m_main_script };
}

// MARK: - Scene Construction

auto kestrel::ui::scene_definition::construct_scene() -> ui::game_scene::lua_reference
{
    // Load up an appropriate builder for the scene here...
    if (m_interface.get() && m_interface->valid()) {
        auto scene = construct_interface();
        if (scene.get()) {
            return scene;
        }
    }

    if (m_dialog.get() && m_dialog->valid()) {
        auto scene = construct_macintosh_dialog();
        if (scene.get()) {
            return scene;
        }
    }

    // Return a scene that isn't a dialog...
    return { new game_scene(m_main_script) };
}

auto kestrel::ui::scene_definition::construct_interface() -> ui::game_scene::lua_reference
{
    auto *L = kestrel::lua_runtime()->internal_state();
    dialog_configuration cfg({ L, m_interface->with_type(scene_interface::resource_type::code) });

    // Do we have an image to apply to the background?
    if (m_background.get() && m_background->valid()) {
        cfg.set_background({ L, m_background });
    }

    // SceneInterface does not require elements to be defined, so proceed straight to constructing the scene.
    game_scene::lua_reference scene(new game_scene(m_main_script));
    cfg.build_into_scene(scene);
    return scene;
}

auto kestrel::ui::scene_definition::construct_macintosh_dialog() -> ui::game_scene::lua_reference
{
    auto *L = kestrel::lua_runtime()->internal_state();
    dialog_configuration cfg({ L, m_dialog->with_type(legacy::macintosh::toolbox::dialog::resource_type::code) });

    // Do we have an image to apply to the background?
    if (m_background.get() && m_background->valid()) {
        cfg.set_background({ L, m_background });
    }

    // Macintosh Dialog needs to have elements defined and mapped correctly.
    for (const auto& element : m_ditl_map) {
        cfg.define_element({ L, element.index }, element.name, element.type);
    }

    // Construct the scene.
    game_scene::lua_reference scene(new game_scene(m_main_script));
    cfg.build_into_scene(scene);
    return scene;
}

auto kestrel::ui::scene_definition::push() -> void
{
    auto scene = construct_scene();
    if (!scene.get()) {
        // TODO: Log and error in loading the scene.
        return;
    }

    kestrel::push_scene(scene);
}

// MARK: - Decoding / Loading

auto kestrel::ui::scene_definition::load() -> void
{
    resource::reader reader(m_descriptor);
    if (reader.valid()) {
        // Read the linked resource references...
        m_main_script = reader.read_resource_reference();
        m_interface = reader.read_resource_reference();
        m_dialog = reader.read_resource_reference();
        m_background = reader.read_resource_reference();
        m_background_top = reader.read_resource_reference();
        m_background_bottom = reader.read_resource_reference();

        // Construct a mapping for the elements in the DITL.
        auto count = reader.read_short();
        for (auto i = 0; i < count; ++i) {
            struct ditl_element_mapping element;
            element.index = reader.read_short();
            element.name = reader.read_pstr();
            element.type = reader.read_byte();
            m_ditl_map.emplace_back(std::move(element));
        }
    }
    else {
        // Something has gone wrong loading the information for the scene definition.
        // TODO: Log an error.
    }
}