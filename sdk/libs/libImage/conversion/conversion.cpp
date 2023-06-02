// Copyright (c) 2023 Tom Hancocks
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

#include <libImage/conversion/conversion.hpp>
#include <libImage/codecs/codec.hpp>
#include <libImage/codecs/png/png.hpp>
#include <libImage/codecs/tga/tga.hpp>
#include <libImage/codecs/pict/pict.hpp>
#include <libImage/codecs/cicn/cicn.hpp>
#include <libImage/codecs/ppat/ppat.hpp>
#include <libImage/codecs/rled/rled.hpp>
#include <libImage/codecs/rlex/rlex.hpp>

auto image::conversion::surface(const std::string& source, const data::block& data) -> quickdraw::surface
{
    auto result = surface(source, std::vector<data::block>({ data }));
    return std::move(result.front());
}

auto image::conversion::surface(const std::string& source, const std::vector<data::block>& data) -> std::vector<quickdraw::surface>
{
    std::vector<quickdraw::surface> surfaces;
    for (const auto& d : data) {
        if (source == format::png) {
            auto ptr = std::make_unique<codec::png>(d);
            surfaces.emplace_back(std::move(ptr->surface()));
        }
        else if (source == format::tga) {
            auto ptr = std::make_unique<codec::tga>(d);
            surfaces.emplace_back(std::move(ptr->surface()));
        }
        else if (source == format::pict) {
            auto ptr = std::make_unique<codec::pict>(d);
            surfaces.emplace_back(std::move(ptr->surface()));
        }
        else if (source == format::cicn) {
            auto ptr = std::make_unique<codec::cicn>(d);
            surfaces.emplace_back(std::move(ptr->surface()));
        }
        else if (source == format::ppat) {
            auto ptr = std::make_unique<codec::ppat>(d);
            surfaces.emplace_back(std::move(ptr->surface()));
        }
    }
    return std::move(surfaces);
}

auto image::conversion::data(const std::string& result, quickdraw::surface& surface) -> data::block
{
    std::vector<quickdraw::surface> surfaces({ surface });
    return data(result, surfaces);
}

auto image::conversion::data(const std::string& result, const std::vector<quickdraw::surface>& surfaces) -> data::block
{
    data::block data;

    if (surfaces.size() == 1) {
        auto surface = surfaces.front();
        if (result == format::png) {
            auto ptr = std::make_unique<codec::png>(surface);
            data = std::move(ptr->data());
        }
        else if (result == format::tga) {
            auto ptr = std::make_unique<codec::tga>(surface);
            data = std::move(ptr->data());
        }
        else if (result == format::pict) {
            auto ptr = std::make_unique<codec::pict>(surface);
            data = std::move(ptr->data());
        }
        else if (result == format::cicn) {
            auto ptr = std::make_unique<codec::cicn>(surface);
            data = std::move(ptr->data());
        }
        else if (result == format::ppat) {
            auto ptr = std::make_unique<codec::ppat>(surface);
            data = std::move(ptr->data());
        }
        else if (result == format::rled) {
            auto ptr = std::make_unique<codec::rled>(surfaces);
            data = std::move(ptr->data());
        }
        else if (result == format::rlex) {
            auto ptr = std::make_unique<codec::rlex>(surfaces);
            data = std::move(ptr->data());
        }
    }
    else if (surfaces.size() > 1) {
        if (result == format::rled) {
            auto ptr = std::make_unique<codec::rled>(surfaces);
            data = std::move(ptr->data());
        }
        else if (result == format::rlex) {
            auto ptr = std::make_unique<codec::rlex>(surfaces);
            data = std::move(ptr->data());
        }
    }
    return std::move(data);
}

auto image::conversion::perform(const std::string& source, const std::string& result, const data::block& data) -> data::block
{
    auto source_surface = conversion::surface(source, data);
    return std::move(conversion::data(result, source_surface));
}

auto image::conversion::perform(const std::string& source, const std::string& result, const std::vector<data::block>& data) -> data::block
{
    std::vector<quickdraw::surface> surfaces;
    for (const auto& d : data) {
        surfaces.emplace_back(surface(source, d));
    }
    return std::move(image::conversion::data(result, surfaces));
}