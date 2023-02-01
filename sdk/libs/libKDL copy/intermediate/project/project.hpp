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

#pragma once

#include <string>
#include <unordered_map>
#include <libKDL/intermediate/metadata/metadata.hpp>
#include <libKDL/intermediate/unit/unit.hpp>

namespace kdl::intermediate
{
    struct project
    {
    public:
        explicit project() = default;

        /**
         * Returns a reference to an intermediate unit based for the specified destination file.
         * @param path The file path for the unit destination.
         * @param allow_creation Should a unit be created if it doesn't already exist?
         * @return A reference to the unit.
         */
        auto unit(const std::string& path, bool allow_creation = true) -> unit&;

        /**
         * Write a new value for the specified metadata key.
         * @param key The metadata key for which to write a value.
         * @param value The value to be stored for the corresponding key.
         */
        auto write_metadata(const metadata::key& key, const std::string& value) -> void;

        /**
         * Read a value from the metadata for the specified key.
         * @param key The metadata key for which to read the value for.
         * @return The value stored for the corresponding key. Will be empty if no value exists.
         */
        [[nodiscard]] auto read_metadata(const metadata::key& key) const -> std::string;

    private:
        std::unordered_map<metadata::key, std::string> m_metadata;
        std::unordered_map<hashing::value, intermediate::unit> m_units;
    };
}