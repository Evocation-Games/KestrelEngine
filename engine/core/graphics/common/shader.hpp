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

#if !defined(KESTREL_SHADER_HPP)
#define KESTREL_SHADER_HPP

#include <string>
#include <memory>

namespace graphics
{

    class shader: public std::enable_shared_from_this<graphics::shader>
    {
    protected:
        std::string m_vertex_code;
        std::string m_fragment_code;
        std::string m_vertex_name;
        std::string m_fragment_name;

    public:
        shader(const std::string& type, const int64_t& vertex_id, const int64_t& fragment_id);

        auto vertex_name() const -> std::string;
        auto fragment_name() const -> std::string;
        auto vertex_code() const -> std::string;
        auto fragment_code() const -> std::string;

        virtual auto use() -> std::shared_ptr<graphics::shader>;
    };

}

#endif //KESTREL_SHADER_HPP
