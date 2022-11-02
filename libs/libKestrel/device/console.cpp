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

#include <unordered_map>
#include <libKestrel/device/console.hpp>

// MARK: - Storage

static struct {
    std::vector<struct kestrel::device::console::record> history;
    std::unordered_map<kestrel::util::uid, std::function<auto(const kestrel::device::console::record&)->void>> pipes;
} s_console;

// MARK: - History

auto kestrel::device::console::write(const std::string &text, enum status status) -> void
{
    struct record item = {
        .status = status,
        .text = text,
    };

    if (s_console.history.size() >= 10000) {
        s_console.history.erase(s_console.history.begin());
    }

    s_console.history.emplace_back(std::move(item));

    for (const auto& pipe : s_console.pipes) {
        pipe.second(s_console.history.back());
    }
}

auto kestrel::device::console::history() -> const std::vector<struct record>&
{
    return s_console.history;
}

// MARK: - Pipe

auto kestrel::device::console::pipe_output(const std::function<auto(const struct record &) -> void> &handler) -> util::uid
{
    util::uid uid;
    s_console.pipes.insert(std::pair(uid, handler));
    return uid;
}

auto kestrel::device::console::destroy_pipe(const util::uid &uid) -> void
{
    auto it = s_console.pipes.find(uid);
    if (it != s_console.pipes.end()) {
        s_console.pipes.erase(it);
    }
}
