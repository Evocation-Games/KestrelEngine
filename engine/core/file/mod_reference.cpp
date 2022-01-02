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
                    .addProperty("userProvided", &mod_reference::user_provided)
                    .addProperty("hasExecuted", &mod_reference::has_executed)
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

auto host::sandbox::mod_reference::user_provided() const -> bool
{
    return m_origin == bundle_origin::user;
}

auto host::sandbox::mod_reference::has_executed() const -> bool
{
    return m_executed;
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

auto host::sandbox::mod_reference::load_modpackage() -> void
{
    if (m_loaded || !m_is_active) {
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
    auto kmod_reader = std::make_shared<graphite::data::reader>(kmod->data());
    m_name = kmod_reader->read_cstr();
    m_version = kmod_reader->read_cstr(0x040);
    m_author = kmod_reader->read_cstr();
    m_primary_namespace = kmod_reader->read_cstr();
    m_lua_entry_script = kmod_reader->read_signed_quad();

    m_loaded = true;
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

auto host::sandbox::mod_reference::load_simplemod() -> void
{
    if (m_loaded || !m_is_active) {
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
    auto kmod_reader = std::make_shared<graphite::data::reader>(kmod->data());
    m_name = kmod_reader->read_cstr();
    m_version = kmod_reader->read_cstr(0x040);
    m_author = kmod_reader->read_cstr();
    m_primary_namespace = kmod_reader->read_cstr();
    m_lua_entry_script = kmod_reader->read_signed_quad();

    m_loaded = true;
}

auto host::sandbox::mod_reference::construct_as_simplemod() -> void
{
    m_name = m_path.substr(m_path.find_last_of('/') + 1);
    m_version = "1.0";
    m_author = "Unknown Author";
    m_primary_namespace = m_name;
}

// MARK: - Mod Lua API

auto host::sandbox::mod_reference::configure_lua_api(const lua_reference& self) -> void
{
    // Make sure we can load the environment before proceeding
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    // and then setup any symbols in the lua environment that will be required.
    env->lua_runtime()->global_namespace()
        .beginNamespace("Kestrel")
            .beginNamespace("Mods")
                .addVariable(m_primary_namespace.c_str(), self.get(), false)
            .endNamespace()
        .endNamespace();
}

