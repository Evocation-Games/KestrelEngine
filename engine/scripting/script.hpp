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

#if !defined(KESTREL_SCRIPT_HPP)
#define KESTREL_SCRIPT_HPP

#include <string>
#include <memory>
#include "scripting/state.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"

namespace scripting::lua
{

    class script
    {
    public:
        struct script_object
        {
            void *data;
            size_t len;
        };

    private:

        constexpr static const char* type { "LuaC" };
        constexpr static const char* script_type { "LuaS" };
        std::weak_ptr<lua::state> m_state;
        int64_t m_id { INT64_MIN };
        std::string m_name;
        std::string m_script;
        struct script_object *m_object { nullptr };

    public:
        script(const std::shared_ptr<lua::state>& state, const asset::resource_descriptor::lua_reference &ref);
        ~script();

        [[nodiscard]] auto id() const ->int64_t;
        [[nodiscard]] auto name() const -> std::string;

        [[nodiscard]] auto code() const -> std::string;
        [[nodiscard]] auto object() const -> void *;
        [[nodiscard]] auto is_object() const -> bool;
        [[nodiscard]] auto object_size() const -> size_t;

        auto execute() const -> void;
    };

}


#endif //KESTREL_SCRIPT_HPP
