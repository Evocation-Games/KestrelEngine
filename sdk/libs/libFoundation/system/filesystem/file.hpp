// Copyright (c) 2019-2022 Tom Hancocks
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
#include <memory>
#include <optional>
#include <sstream>

#include <libFoundation/system/filesystem/file_mode.hpp>
#include <libFoundation/system/filesystem/path.hpp>

namespace foundation::filesystem
{

    /**
     * The filesystem::file structure represents a textual/binary file on the host system.
     * This file could have been read from disk, or be in the process of being constructed.
     */
    struct file
    {
    public:
        /**
         * Create a blank file for writing to.
         */
        file() = default;

        /**
         * Clean up the file and ensure it is safely discarded.
         */
        ~file();

        /**
         * Read the specified file from disk.
         * @param path      The path from with the load the file.
         * @param mode      The mode in which to read the file, so that content is
         *                  interpreted correctly.
         */
        explicit file(filesystem::path path, file_mode mode = file_mode::text);

        /**
         * Create an in memory file, with the specified name and contents. This will create a
         * text mode file.
         */
        file(const std::string& name, const std::string& contents);

        /**
         * Does the file exist in a populated manner (i.e. it has none zero contents)
         * @return  True if the file exists.
         */
         [[nodiscard]] auto exists() const -> bool;

        /**
         * The location of the file on disk. Will be empty if this is a blank file.
         * @return  A path representing the location of the file on disk.
         */
        [[nodiscard]] auto path() const -> filesystem::path;

        /**
         * The contents of the file as a string.
         * @return  Returns the contents of the file as a string.
         */
        [[nodiscard]] auto string_contents() const -> const std::string&;

        /**
         * The contents of the file as a vector of characters.
         * @return  Returns the contents of the file as a vector of characters.
         */
        [[nodiscard]] auto characters() const -> std::vector<char>;

        /**
         * The contents of the file as a vector of bytes.
         * @return  Returns the contents of the file as a vector of bytes.
         */
        [[nodiscard]] auto bytes() const -> std::vector<std::uint8_t>;

        /**
         * Set the contents of the file from a string.
         * @param contents  The string to set the contents of the file to.
         */
        auto set_string_contents(const std::string& contents) -> void;

        /**
         * Set the contents of the file from a vector of characters.
         * @param contents  The vector of characters to set the contents of the file to.
         */
        auto set_characters(const std::vector<char>& contents) -> void;

        /**
         * Set the contents of the file from a vector of bytes.
         * @param contents  The vector of bytes to set the contents of the file to.
         */
        auto set_bytes(const std::vector<std::uint8_t>& contents) -> void;

        /**
         * Save the contents of the file to disk, using a text mode.
         * @warning         This is not compatible with binary data.
         * @param path      The location of the file in which the file contents should be saved to.
         */
        auto save(const filesystem::path& path = {}) -> void;

    private:
        filesystem::path m_path;
        std::uint8_t *m_raw { nullptr };
        std::streamsize m_length { 0 };

        struct {
            std::string string_contents;
        } m_cache;
    };

};
