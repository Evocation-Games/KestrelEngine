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

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/script.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/event/responder/responder_chain.hpp>
#include <libKestrel/clock/timed_event.hpp>
#include <libKestrel/entity/entity.hpp>

namespace kestrel::ui
{
    class scene
    {
    public:
        explicit scene(const lua::script& script);

        [[nodiscard]] auto is_passthrough_render() const -> bool;
        auto set_passthrough_render(bool f) -> void;

        [[nodiscard]] auto scaling_factor() const -> double;
        auto set_scaling_factor(double f) -> void;

        auto add_update_block(const std::function<auto(const rtc::clock::duration&)->void>& block) -> void;
        auto invoke_update_blocks(const rtc::clock::duration& delta) -> void;

        auto add_render_block(const std::function<auto()->void>& block) -> void;
        auto invoke_render_blocks() -> void;

        auto receive_event(const event& event) -> void;
        auto add_key_event_block(const std::function<auto(const event&)->void>& block) -> void;
        auto add_mouse_event_block(const std::function<auto(const event&)->void>& block) -> void;

        auto add_timed_event(const std::shared_ptr<rtc::timed_event>& event) -> void;
        auto check_timed_events() -> void;

        [[nodiscard]] auto current_time() const -> double;

        auto assign_key_responder(const std::shared_ptr<responder_chain::key_responder>& responder) -> bool;
        [[nodiscard]] auto key_responder() const -> std::shared_ptr<responder_chain::key_responder>;

        auto start() -> void;
        auto update(const rtc::clock::duration& delta) -> void;
        auto render() -> void;

        auto draw_entity(const std::shared_ptr<ecs::entity>& entity) const -> void;

    private:
        lua::script m_script;
        std::vector<std::function<auto()->void>> m_render_blocks;
        std::vector<std::function<auto(const rtc::clock::duration&)->void>> m_update_blocks;
        std::vector<std::function<auto(const event&)->void>> m_key_event_blocks;
        std::vector<std::function<auto(const event&)->void>> m_mouse_event_blocks;
        std::vector<std::shared_ptr<rtc::timed_event>> m_timed_events;
        std::weak_ptr<responder_chain::key_responder> m_key_responder;
        rtc::clock::time m_starting_time { rtc::clock::global().current() };
        bool m_passthrough_render { false };
        double m_scaling_factor { 1.f };
    };

}
