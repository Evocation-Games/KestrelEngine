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

#include <map>
#include <string>
#include <optional>
#include <functional>
#include <libFoundation/system/filesystem/path.hpp>

namespace kdtool::builder::directory
{
    struct entry
    {
        explicit entry(const std::string& name)
            : m_name(name)
        {}

        [[nodiscard]] auto name() const -> std::string { return m_name; }
        [[nodiscard]] auto has_file() const -> bool { return m_path.has_value(); }
        [[nodiscard]] auto path() const -> foundation::filesystem::path { return m_path.value(); }
        [[nodiscard]] auto is_leaf() const -> bool { return m_children.empty(); }

        auto each(const std::function<auto(const std::shared_ptr<entry>&)->void>& callback) -> void
        {
            for (const auto& it : m_children) {
                callback(it.second);
            }
        }

        auto add(const std::shared_ptr<entry>& entry) -> void
        {
            if (entry) {
                m_children.emplace(entry->name(), entry);
            }
        }

    private:
        std::string m_name;
        std::optional<foundation::filesystem::path> m_path;
        std::map<std::string, std::shared_ptr<struct entry>> m_children;
    };
}