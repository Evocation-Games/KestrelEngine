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
#include <string>

#if !defined(lua_api)
#   define lua_api(...)
#endif

#if !defined(has_named_constructable_lua_api)
#   define has_named_constructable_lua_api(_type)  \
        typedef luabridge::RefCountedPtr<_type> lua_reference; \
        static auto enroll_object_api_in_state(const std::string& name, const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void
#endif

#if !defined(has_contructable_lua_api)
#   define has_contructable_lua_api(_type)  \
        typedef luabridge::RefCountedPtr<_type> lua_reference; \
        static auto enroll_object_api_in_state(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void
#endif

#if !defined(contruct_lua_api)
#   define contruct_lua_api(_type)  \
        auto _type::enroll_object_api_in_state(const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void
#endif

#if !defined(contruct_named_lua_api)
#   define contruct_named_lua_api(_type)  \
        auto _type::enroll_object_api_in_state(const std::string& name, const std::shared_ptr<kestrel::lua::runtime>& runtime) -> void
#endif