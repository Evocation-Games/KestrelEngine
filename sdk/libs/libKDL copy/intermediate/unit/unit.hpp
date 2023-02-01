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
#include <libHashing/hashing.hpp>
#include <libKDL/intermediate/metadata/metadata.hpp>
#include <libKDL/intermediate/named_container/named_container.hpp>
#include <libGraphite/rsrc/file.hpp>

namespace kdl::intermediate
{
    /**
     * The `kdl::intermediate::unit` is an in memory representation of a fully assembled resource file. It is the
     * _intermediate_ step between KDL source code and a Resource File.
     */
    struct unit
    {
    public:
        /**
         * Construct a unit for the specified destination file.
         * @param path
         */
        explicit unit(const std::string& path);

        /**
         * A hash value representing this specific unit.
         * @return The hash value of the destination file path.
         */
        [[nodiscard]] auto hash() const -> hashing::value;

        /**
         * The current required format for the resource file.
         * @return The resource format required for the output file.
         */
        [[nodiscard]] auto format() const -> enum graphite::rsrc::file::format;

        /**
         * Returns a reference to the specified named container. If the container does not yet have the container,
         * then it will be created.
         * @param name The name of the container to find.
         * @param allow_creation Should the container be created if it is not found.
         * @return A reference to a named container.`
         */
        auto container(const std::string& name, bool allow_creation = true) -> intermediate::named_container&;

        /**
         * Returns a reference to the global named container.
         * @return A reference to a named container representing the global container.
         */
        auto global_container() -> intermediate::named_container&;



    private:
        std::string m_destination_file_path;
        enum graphite::rsrc::file::format m_format { graphite::rsrc::file::format::extended };
        std::unordered_map<hashing::value, intermediate::named_container> m_containers;
        std::unordered_map<metadata::key, std::string> m_metadata;
    };
}