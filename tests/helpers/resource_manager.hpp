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

#include <libGraphite/rsrc/manager.hpp>

namespace test
{
    struct resource_manager
    {
        static auto setup() -> resource_manager&
        {
            static resource_manager instance;
            tear_down();
            return instance;
        }

        static auto tear_down() -> void
        {
            graphite::rsrc::manager::shared_manager().tear_down();
        }

        auto add_file(const std::string& name = "TestFile.kdat") -> resource_manager&
        {
            m_current_file = graphite::rsrc::manager::shared_manager().import_file(new graphite::rsrc::file());
            return *this;
        }

        auto add_resource(const std::string& type_code,
                          graphite::rsrc::resource::identifier id,
                          const std::string& name,
                          const std::string& namespace_name) -> resource_manager&
        {
            if (namespace_name.empty()) {
                m_current_file->add_resource(type_code, id, name, graphite::data::block());
            }
            else {
                m_current_file->add_resource(type_code, id, name, graphite::data::block(), {
                    std::pair(kestrel::resource::resource_namespace::attribute_name, namespace_name)
                });
            }
            return *this;
        }

    private:
        graphite::rsrc::file *m_current_file { nullptr };
    };
}