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
#include <libResource/definition/type/instance.hpp>
#include <libResource/reference.hpp>
#include <libResource/declaration/value.hpp>
#include <libInterpreter/token/token.hpp>
#include <libGraphite/data/data.hpp>

namespace resource
{
    struct instance
    {
        instance() = default;
        explicit instance(const resource::reference& reference);
        instance(const resource::reference& reference, graphite::data::block& data);

        [[nodiscard]] auto reference() const -> resource::reference;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto type() const -> const definition::type::instance *;
        [[nodiscard]] auto values() const -> const std::unordered_map<std::string, value_container>&;
        [[nodiscard]] auto data() const -> const graphite::data::block&;

        auto set_reference(const resource::reference& reference) -> void;
        auto set_name(const std::string& name) -> void;

        auto set_value(const std::string& key, const value_container& value) -> void;
        auto set_values(const std::unordered_map<std::string, value_container>& values) -> void;

    private:
        resource::reference m_reference;
        std::string m_name;
        const definition::type::instance *m_type { nullptr };
        std::unordered_map<std::string, value_container> m_values;
        graphite::data::block m_data;
    };
}