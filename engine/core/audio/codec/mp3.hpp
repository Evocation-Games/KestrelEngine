// Copyright (c) 2021 Tom Hancocks
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

#if !defined(AUDIO_IMA4_DECODER_HPP)
#define AUDIO_IMA4_DECODER_HPP

#include <memory>
#include <string>
#include "core/audio/player/player_item.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace audio::asset
{

    class mp3 : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<audio::asset::mp3> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        lua_api explicit mp3(const std::string& file_path);

        lua_api auto play() -> void;
        lua_api auto stop() -> void;

    private:
        uint64_t m_ref { 0 };
        std::shared_ptr<audio::player_item> m_item { nullptr };
        std::string m_path { "" };
    };

}

#endif
