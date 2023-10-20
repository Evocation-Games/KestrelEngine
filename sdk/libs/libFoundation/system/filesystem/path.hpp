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

#include <string>
#include <vector>
#include <cstdint>
#include <initializer_list>
#include <libFoundation/availability.hpp>
#include <libFoundation/system/filesystem/file_type.hpp>

namespace foundation::filesystem
{
    /**
     * Represents a filesystem path on the host system.
     */
    API_VERSION(1.0)
    struct path
    {
    public:
        path() = default;

        /**
         * Construct a new path.
         * @param str           Host system path using slash '/' delimited path components.
         */
        explicit path(const std::string& str);

        /**
         * Construct a new path.
         * @param components    A list of singular path components.
         * @param is_relative   Is the path relative to the current working directory?
         */
        path(const std::initializer_list<std::string>& components, bool is_relative = false, const std::string& scheme = "");

        /**
         * Construct a new path.
         * @param components    A list of singular path components.
         * @param is_relative   Is the path relative to the current working directory?
         */
        explicit path(const std::vector<std::string>& components, bool is_relative = false, const std::string& scheme = "");

        /**
         * Takes a host system path using slash '/' delimited path components, and breaks them apart
         * into a vector of path components.
         * @param path      Host system path using slash '/' delimited path components.
         * @param separator An alternate path separator to use. Defaults to '/'.
         * @return          Vector of path components
         */
        static auto path_components(const std::string& path, char separator = '/') -> std::vector<std::string>;

        /**
         * Takes a host system path, and determines if either an http://, https:// or file:// scheme is prefixed
         * to it, and then returns the scheme.
         * @param path      Host system path using slash '/' delimited path components.
         * @return          The scheme present in the path (empty string if no scheme)
         */
        static auto path_scheme(const std::string& path) -> std::string;

        /**
         * Creates a path that represents the location of configuration files for the user.
         */
        static auto configuration_directory(const std::string& name = "kestrel") -> path;

        /**
         * Creates a new string by appending the specified path component to it.
         * @return
         */
        [[nodiscard]] auto appending_path_component(const std::string& component) const -> path;

        /**
         * Access the string representation of the path.
         * @return
         */
        [[nodiscard]] auto string() const -> std::string;

        /**
         * Access the internal C String representation of the path.
         * @return
         */
        [[nodiscard]] auto c_str() const -> const char *;

        /**
         * The name of the currently represented directory in the path. If the last path component is a directory,
         * then this will be the same as _name()_.
         * @return      The name of the current directory.
         */
        [[nodiscard]] auto directory_name() const -> std::string;

        /**
         * The name of the filesystem item represented by the path.
         * @return      The name of the represented filesystem item.
         */
        [[nodiscard]] auto name() const -> std::string;

        /**
         * Reports the current type of the represented item in the host file system.
         * @return      The file item type of the represented item.
         */
        [[nodiscard]] auto type() const -> file_type;

        /**
         * Is the represented file item type a directory?
         * @return      Is the item a directory or not?
         */
        [[nodiscard]] auto is_directory() const -> bool;

        /**
         * Does the represented file item actually exist within the host file system.
         * @return      Does the item exist or not?
         */
        [[nodiscard]] auto exists() const -> bool;

        /**
         * The number of components that make up the path.
         * @return      The number of components in the path.
         */
        [[nodiscard]] auto component_count() const -> std::size_t;

        /**
         * Return the components of the path.
         */
        [[nodiscard]] auto components() const -> std::vector<std::string>;

        /**
         * Does the path contain any components, and thus represent anything on disk.
         * @return       The result of the query.
         */
        [[nodiscard]] auto empty() const -> bool;

        /**
         * Touch the file item in the host filesystem.
         */
        auto touch() -> void;

        /**
         * Create a directory at the file path.
         * @param ignore_last       Should the last component in the path be ignored?
         * @return                  Could the directory at path be successfully created?
         */
        auto create_directory(bool ignore_last = true) const -> bool;

        /**
         * Create a new path for the child specified.
         * @param name  The name of the child to be added to the path.
         * @return      A file system item for a child item.
         */
        [[nodiscard]] auto child(const std::string& name) const -> path;

        /**
         * The path of the parent directory.
         * @return      The parent directory path.
         */
        [[nodiscard]] auto parent() const -> path;

        /**
         * Copy the file at the represented path, to the specified destination path.
         * @param path   The destination path to copy the file to.
         * @return       Was the copy operation successful?
         */
        [[nodiscard]] auto copy_to(const path& path) const -> bool;

        /**
         * Replace a path component with the contents of another path.
         */
        [[nodiscard]] auto replace_component(std::int32_t i, const path& replacement) const -> path;

        /**
         * Iterate over each of the children in the directory.
         * @param fn
         */
        auto each_child(const std::function<auto(const path&)->void>& fn) const -> void;

        /**
         * Check to see if the path has the specified extension.
         * @param ext The extension to test for (excluding dot)
         * @return
         */
        [[nodiscard]] auto has_extension(const std::string& ext) const -> bool;

    public:
        /**
         * Check if the item at the specified path exists or not.
         * @param path  The file path of an item in the host file system.
         * @return      The result of whether the item exists or not.
         */
        static auto exists(const path& path) -> bool;

        /**
         * Check if the item at the specified path is a directory or not.
         * @param path  The file path of an item in the host file system.
         * @return      The result of whether the item exists or not.
         */
        static auto is_directory(const path& path) -> bool;

        /**
         * Resolve the `~` tilde in the path to the users home directory in the host file path.
         * @param path  The file path of an item, using the tilde shorthand.
         * @return      The resolved file path.
         */
        static auto resolve_tilde(const path& path) -> filesystem::path;

        /**
         * Create the directory at the specified path.
         * @param path  The file path of the directory to be created.
         */
        static auto make_directory(const path& path) -> void;

        /**
         * Determine if the path string specified is absolute to the host start up volume.
         * @param path  The file path to be checked.
         * @return      Is the file path absolute?
         */
        static auto is_absolute_path(const std::string& path) -> bool;

    private:
        auto convert_to_absolute() -> void;

    private:
        bool m_relative { false };
        std::string m_path_buffer;
        std::vector<std::string> m_components;
        std::string m_scheme;
    };

}