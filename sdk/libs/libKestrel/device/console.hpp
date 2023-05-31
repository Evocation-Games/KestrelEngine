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

#include <vector>
#include <string>
#include <functional>
#include <libKestrel/util/uid.hpp>
#include <libKestrel/entity/entity.hpp>
#include <libKestrel/event/event.hpp>

namespace kestrel::device::console
{
    /**
     * Indicate the status type of a log record that has been posted.
     * - normal     A normal log record.
     * - success    A log record indicating a successful event.
     * - error      A log record indicating an error state.
     * - warning    A log record indicating that a warning has been raised.
     * - note       A log record indicating that a note has been posted.
     * - input      A log record of input from the user.
     */
    enum class status { normal, success, error, warning, note, input };

    /**
     * A record structure tracks information posted to the console.
     */
    struct record
    {
        /**
         * The type of information being represented in the record.
         */
        enum status status { status::normal };

        /**
         * The content of the record as a string to be displayed in the console.
         */
        std::string text;
    };

    /**
     * Clear all records from the console history.
     */
    auto clear_history() -> void;

    /**
     * Reports all of the currently recorded records in the console history.
     * @return  A vector of records.
     */
    auto history() -> const std::vector<struct record>&;

    /**
     * Write a new record into the console history.
     * @param text    The string content of the record.
     * @param status  The type of record being recorded.
     */
    auto write(const std::string& text, enum status status = status::normal) -> void;

    /**
     * Request updates on all records being added to the console.
     * @param handler A callback that is called with each of the records added to the console.
     * @return An identifier representing the created "pipe".
     */
    auto pipe_output(const std::function<auto(const struct record&)->void>& handler) -> util::uid;

    /**
     * Destroy the specified "pipe".
     * @param uid An identifier representing the pipe to be destroyed.
     */
    auto destroy_pipe(const util::uid& uid) -> void;

    [[nodiscard]] auto entity() -> std::shared_ptr<ecs::entity>;
    auto show_console() -> void;
    auto hide_console() -> void;
    auto toggle_console() -> void;
    auto send_event(const event& e) -> bool;
    auto redraw() -> void;
}