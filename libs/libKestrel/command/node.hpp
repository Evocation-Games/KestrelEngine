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

#include <memory>
#include <vector>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/command/argument.hpp>
#include <libGraphite/util/hashing.hpp>

namespace kestrel::command
{
    struct lua_api(Kestrel.Command.Node, Available_0_9) node
    {
    public:
        has_constructable_lua_api(node);

        explicit node(graphite::hashing::value64 name, node *parent = nullptr);

        auto add_argument(argument&& arg) -> void;

        [[nodiscard]] auto name_hash() const -> graphite::hashing::value64;
        [[nodiscard]] auto is_named(const std::string& name) const -> bool;
        [[nodiscard]] auto is_named(graphite::hashing::value64 value) const -> bool;

        [[nodiscard]] auto argument_count() const -> std::int32_t;
        [[nodiscard]] auto argument_at(std::int32_t idx) const -> const argument&;
        [[nodiscard]] auto has_argument(graphite::hashing::value64 value, std::int32_t idx = -1) const -> bool;

        [[nodiscard]] auto has_children() const -> bool;
        [[nodiscard]] auto children() const -> std::vector<node::lua_reference>;

    private:
        node *m_parent { nullptr };
        graphite::hashing::value64 m_name;
        std::vector<argument> m_arguments;
        std::vector<node::lua_reference> m_children;
    };
}