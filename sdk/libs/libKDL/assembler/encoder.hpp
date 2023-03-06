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

#include <libResource/declaration/instance.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libGraphite/data/writer.hpp>

namespace kdl::assembler
{
    struct encoder
    {
        explicit encoder(const resource::instance& instance, const resource::definition::type::instance *type);

        auto encode() -> const graphite::data::block&;

    private:
        const resource::instance *m_instance_ref { nullptr };
        const resource::definition::type::instance *m_type { nullptr };
        graphite::data::writer m_writer;

        auto encode_binary_template(const std::string& prefix, const resource::definition::binary_template::instance *binary_template) -> void;
        auto encode_binary_field(const std::string& prefix, const resource::definition::binary_template::field& field) -> void;
        auto encode_value(const resource::value_container& value, const resource::definition::binary_template::type& type) -> void;
    };
}