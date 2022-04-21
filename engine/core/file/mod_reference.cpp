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

#include <libGraphite/rsrc/file.hpp>
#include <libGraphite/data/reader.hpp>
#include <libGraphite/rsrc/manager.hpp>
#include "core/file/mod_reference.hpp"
#include "core/file/file_reference.hpp"
#include "core/file/directory_reference.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto host::sandbox::mod_reference::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Kestrel")
            .beginNamespace("Sandbox")
                .beginClass<host::sandbox::mod_reference>("ModReference")
                    .addProperty("name", &mod_reference::name)
                    .addProperty("version", &mod_reference::version)
                    .addProperty("author", &mod_reference::author)
                    .addProperty("path", &mod_reference::path)
                    .addProperty("primaryNamespace", &mod_reference::primary_namespace)
                    .addProperty("userProvided", &mod_reference::user_provided)
                    .addProperty("isLoaded", &mod_reference::is_loaded)
                    .addProperty("hasExecuted", &mod_reference::has_executed)
                    .addProperty("description", &mod_reference::description)
                    .addProperty("enabled", &mod_reference::enabled, &mod_reference::set_enabled)
                    .addProperty("category",  &mod_reference::category)
                    .addProperty("isScenario", &mod_reference::is_scenario)
                    .addProperty("packageID", &mod_reference::package_id)
                    .addProperty("scenarioID", &mod_reference::scenario_id)
                    .addFunction("loadResources", &mod_reference::lua_load_resources)
                    .addFunction("execute", &mod_reference::lua_execute)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

host::sandbox::mod_reference::mod_reference(const std::string &path, bundle_origin origin, bundle_type type)
    : m_path(path), m_origin(origin), m_bundle(type), m_is_active(true)
{
}

// MARK: - Accessors

auto host::sandbox::mod_reference::has_initial_script() const -> bool
{
    return m_lua_entry_script >= 0;
}

auto host::sandbox::mod_reference::name() const -> std::string
{
    return m_name;
}

auto host::sandbox::mod_reference::author() const -> std::string
{
    return m_author;
}

auto host::sandbox::mod_reference::version() const -> std::string
{
    return m_version;
}

auto host::sandbox::mod_reference::path() const -> std::string
{
    return m_path;
}

auto host::sandbox::mod_reference::primary_namespace() const -> std::string
{
    return m_primary_namespace;
}

auto host::sandbox::mod_reference::description() const -> std::string
{
    return m_description;
}

auto host::sandbox::mod_reference::user_provided() const -> bool
{
    return m_origin == bundle_origin::user;
}

auto host::sandbox::mod_reference::has_executed() const -> bool
{
    return m_executed;
}

auto host::sandbox::mod_reference::is_loaded() const -> bool
{
    return m_loaded;
}

auto host::sandbox::mod_reference::category() const -> std::string
{
    return m_category;
}

auto host::sandbox::mod_reference::package_id() const -> std::string
{
    return m_package_id;
}

auto host::sandbox::mod_reference::scenario_id() const -> std::string
{
    return m_scenario_id;
}

auto host::sandbox::mod_reference::is_scenario() const -> bool
{
    return m_category == "scenario";
}

auto host::sandbox::mod_reference::enabled() const -> bool
{
    return m_enabled;
}

auto host::sandbox::mod_reference::set_enabled(bool f) -> void
{
    m_enabled = f;
}

// MARK: - Mod Package

auto host::sandbox::mod_reference::validate_as_modpackage() const -> bool
{
    // Load the mod as a file reference and confirm that it is a directory.
    if (!file_reference(m_path).is_directory()) {
        return false;
    }

    // Load the mod as a directory reference, and attempt to load the package.rsrc file that contains
    // the basic mod package information.
    directory_reference mod_dir { m_path };
    auto package_rsrc_ref = mod_dir.file("package.rsrc");
    if (!package_rsrc_ref.get() && package_rsrc_ref->exists()) {
        return false;
    }

    // We have the file, now load it and make sure we have a legit resource file, with appropriate resources.
    auto package_rsrc = std::make_shared<graphite::rsrc::file>(package_rsrc_ref->path());
    auto kmod = package_rsrc->find("kmöd", 128, {}).lock();
    if (!kmod) {
        return false;
    }

    // At this point we can safely say that the mod is valid.
    return true;
}

auto host::sandbox::mod_reference::parse_modpackage() -> void
{
    if (!m_is_active || m_parsed) {
        return;
    }

    // Make sure we can load the environment before proceeding, otherwise we'll struggle to load the
    // mod correctly.
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    // Load the mod as a directory reference, and attempt to load the package.rsrc file that contains
    // the basic mod package information.
    directory_reference mod_dir { m_path };
    auto package_rsrc_ref = mod_dir.file("package.rsrc");
    if (!package_rsrc_ref.get() && package_rsrc_ref->exists()) {
        m_is_active = false;
        return;
    }

    // We have the file, now load it and make sure we have a legit resource file, with appropriate resources.
    auto package_rsrc = std::make_shared<graphite::rsrc::file>(package_rsrc_ref->path());
    auto kmod = package_rsrc->find("kmöd", 128, {}).lock();
    if (!kmod) {
        m_is_active = false;
        return;
    }

    // Load the mod meta data...
    m_mod_files.emplace_back(package_rsrc);

    auto kmod_reader = std::make_shared<graphite::data::reader>(kmod->data());
    m_name = kmod_reader->read_cstr();
    m_version = kmod_reader->read_cstr(0x040);
    m_author = kmod_reader->read_cstr();
    m_primary_namespace = kmod_reader->read_cstr();
    m_lua_entry_script = kmod_reader->read_signed_quad();
    m_description = kmod_reader->read_cstr();
    m_category = kmod_reader->read_cstr();
    m_package_id = kmod_reader->read_cstr();
    m_scenario_id = kmod_reader->read_cstr();
    m_parsed = true;
}


// MARK: - Mod Package

auto host::sandbox::mod_reference::validate_as_simplemod() const -> bool
{
    // We have the file, now load it and make sure we have a legit resource file, with appropriate resources.
    auto rsrc = std::make_shared<graphite::rsrc::file>(m_path);
    auto kmod = rsrc->find("kmöd", 128, {}).lock();
    if (!kmod) {
        return false;
    }

    // At this point we can safely say that the mod is valid.
    return true;
}

auto host::sandbox::mod_reference::parse_simplemod() -> void
{
    if (!m_is_active || m_parsed) {
        return;
    }

    // We have the file, now load it and make sure we have a legit resource file, with appropriate resources.
    auto rsrc = std::make_shared<graphite::rsrc::file>(m_path);
    auto kmod = rsrc->find("kmöd", 128, {}).lock();
    if (!kmod) {
        m_is_active = false;
        return;
    }

    // Load the mod meta data...
    m_mod_files.emplace_back(rsrc);

    auto kmod_reader = std::make_shared<graphite::data::reader>(kmod->data());
    m_name = kmod_reader->read_cstr();
    m_version = kmod_reader->read_cstr(0x040);
    m_author = kmod_reader->read_cstr();
    m_primary_namespace = kmod_reader->read_cstr();
    m_lua_entry_script = kmod_reader->read_signed_quad();
    m_description = kmod_reader->read_cstr();
    m_parsed = true;
}

auto host::sandbox::mod_reference::construct_simplemod() -> void
{
    if (!m_is_active || m_parsed) {
        return;
    }

    m_name = m_path.substr(m_path.find_last_of('/') + 1);
    m_version = "1.0";
    m_author = "Unknown Author";
    m_primary_namespace = m_name;
    m_parsed = true;
    m_description = "No Description";
    m_category = "plugin";
    m_scenario_id = "";
    m_package_id = "";

    m_mod_files.emplace_back(std::make_shared<graphite::rsrc::file>(m_path));
}

// MARK: - Load & Execution

auto host::sandbox::mod_reference::lua_load_resources() -> void
{
    // NOTE: This is a horrible work around to an issue with LuaBridge. Attempts to add a reference the mod
    // as a property within the Kestrel.Mods namespace results in strange copy on write behaviour, meaning
    // operations are discarded immediately.
    // This approach finds the actual true mod reference, and the directly executes it.
    const auto& mods = sandbox::files::shared_files().mods();
    for (auto i = 0; i < mods.size(); ++i) {
        const auto& mod = mods.at(i);
        if (mod->path() == m_path) {
            mod->load_resources();
        }
    }
}

auto host::sandbox::mod_reference::load_resources() -> void
{
    if (m_enabled) {
        return;
    }

    if (!m_is_active) {
        if (auto env = environment::active_environment().lock()) {
            env->lua_out("The mod '" + m_name + "' is not active, so it can not be loaded.", true);
        }
        return;
    }

    if (m_loaded) {
        if (auto env = environment::active_environment().lock()) {
            env->lua_out("The mod '" + m_name + "' is already loaded.", true);
        }
        return;
    }

    for (const auto& f : m_mod_files) {
        graphite::rsrc::manager::shared_manager().import_file(f);
    }

    m_loaded = true;
}

auto host::sandbox::mod_reference::lua_execute() -> void
{
    // NOTE: This is a horrible work around to an issue with LuaBridge. Attempts to add a reference the mod
    // as a property within the Kestrel.Mods namespace results in strange copy on write behaviour, meaning
    // operations are discarded immediately.
    // This approach finds the actual true mod reference, and the directly executes it.
    const auto& mods = sandbox::files::shared_files().mods();
    for (auto i = 0; i < mods.size(); ++i) {
        const auto& mod = mods.at(i);
        if (mod->path() == m_path) {
            mod->execute();
            return;
        }
    }
}

auto host::sandbox::mod_reference::execute() -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    if (m_enabled) {
        return;
    }

    if (!m_is_active) {
        env->lua_out("The mod '" + m_name + "' is not active, so it can not be executed.", true);
        return;
    }

    if (!m_loaded) {
        env->lua_out("The mod '" + m_name + "' is not loaded, so it can not be executed.", true);
        return;
    }

    if (m_executed) {
        env->lua_out("The mod '" + m_name + "' has already been executed, and can not be executed again.", true);
        return;
    }

    // Try and find the initial script that was requested.
    const auto& file = m_mod_files.at(0);
    if (const auto& ref = file->find("LuaS", m_lua_entry_script, { std::make_pair("namespace", m_primary_namespace) }).lock()) {
        scripting::lua::script script { env->lua_runtime(), ref };
        script.execute();
    }
    else if (const auto& ref = file->find("LuaS", m_lua_entry_script, {}).lock()) {
        scripting::lua::script script { env->lua_runtime(), ref };
        script.execute();
    }
    else {
        env->lua_out("Could not execute mod entry script.");
        return;
    }

    m_executed = true;
}

