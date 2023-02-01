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
#include <libKDL/intermediate/resource_container/resource_container.hpp>

namespace kdl::intermediate
{
    /**
     * The `kdl::intermediate::named_container` structure represents a "namespace" within a resource file,
     * and is used to help categorize resources and reduce potential collisions from happening.
     *
     * Some default containers/namespaces are:
     *  - Global (This the only container available under Classic and Rez formats)
     */
    struct named_container
    {
    public:
        explicit named_container(const std::string& name);

        /**
         * Constructs a new named_container representing the "Global" named container.
         * @return A named container representing the global container.
         */
        static auto global() -> named_container;

        /**
         * The name of the container.
         * @return The name of the container as a string.
         */
        [[nodiscard]] auto name() const -> std::string;

        /**
         * Merge the provided named container into this container.
         * @param container The container to merge into this one.
         */
        auto merge(const named_container& container) -> void;

        /**
         * Returns a reference to the specified intermediate type container, or creates it if it does not
         * yet exist.
         * @param code The 'type code' of the resource container.
         * @param allow_creation Should a resource container be created if it does not exist.
         * @return A reference to the specified intermediate type container.
         */
        auto resource_container(const std::string& code, bool allow_creation = true) -> intermediate::resource_container&;

    public:
        static constexpr const char *global_container_name = "";

    private:
        std::string m_name;
        std::unordered_map<hashing::value, intermediate::resource_container> m_types;
    };
}