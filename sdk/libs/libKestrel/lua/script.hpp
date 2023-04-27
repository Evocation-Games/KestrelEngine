// Copyright (c) 2020 Tom Hancocks
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
#include <memory>

#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/macro.hpp>

namespace kestrel::lua
{
    class runtime;

    class script
    {
    public:
        is_resource_type("LuaS");

        enum class format { source, bytecode };

        struct chunk_info {
            void *ptr;
            std::size_t size;
        };

        script() = default;
        script(const std::shared_ptr<runtime>& runtime, const resource::descriptor::lua_reference &ref);
        script(const std::shared_ptr<runtime>& runtime, const graphite::rsrc::resource *resource);
        script(const std::shared_ptr<runtime>& runtime, const graphite::data::block& data);
        script(const std::shared_ptr<runtime>& runtime, const std::string& script);

        [[nodiscard]] auto id() const -> graphite::rsrc::resource::identifier;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto code() const -> std::string;
        [[nodiscard]] auto bytecode() const -> void *;
        [[nodiscard]] auto bytecode_size() const -> std::size_t;
        [[nodiscard]] auto format() const -> enum format;

        auto execute() const -> void;

        auto read_next_chunk() -> chunk_info;

    private:
        std::weak_ptr<runtime> m_runtime;
        graphite::rsrc::resource::identifier m_id { INT64_MIN };
        std::string m_name;
        std::string m_script;
        void *m_bytecode { nullptr };
        std::size_t m_bytecode_size { 0 };
        std::size_t m_bytecode_offset { 0 };
        enum format m_format;
    };

}
