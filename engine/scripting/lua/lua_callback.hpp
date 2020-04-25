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

#if !defined(KESTREL_LUA_CALLBACK_HPP)
#define KESTREL_LUA_CALLBACK_HPP

#include "scripting/lua/lua.hpp"

namespace kestrel { namespace lua {

    class callback: public lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<lua::callback> lua_callback;
        enum type { none, function, script };

    private:
        std::string m_name { "" };
        int64_t m_id { INT64_MIN };
        enum callback::type m_type { none };

    public:
        static auto register_object() -> void;

        static auto named_function(std::string name) -> lua_callback;
        static auto script_resource(int64_t id) -> lua_callback;

        callback();
        callback(std::string name);
        callback(int64_t id);

        auto call() -> void;
    };

}};

#endif //KESTREL_LUA_CALLBACK_HPP
