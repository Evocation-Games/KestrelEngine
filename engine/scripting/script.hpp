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

namespace scripting { namespace lua {

    class script
    {
    private:
        constexpr static const char* type { "LuaC" };
        std::weak_ptr<lua::state> m_state;
        int64_t m_id { INT64_MIN };
        std::string m_name { "" };
        std::string m_script { "" };

    public:
        script(std::shared_ptr<lua::state> state, const int64_t& id);

        auto code() const -> std::string;
        auto execute() const -> void;
    };

}}


#endif //KESTREL_SCRIPT_HPP
