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

#include <libSound/conversion/conversion.hpp>
#include <libSound/codecs/snd/snd.hpp>
#include <libSound/codecs/wav/wav.hpp>

auto sound::conversion::samples(const std::string& source, const data::block& data) -> sample_information
{
    sample_information samples;
    if (source == format::snd) {
        auto ptr = std::make_unique<codec::snd>(data);
        samples.samples = std::move(ptr->samples());
        samples.channels = ptr->channels();
        samples.bits = ptr->sample_bits();
        samples.rate = ptr->sample_rate();
    }
    else if (source == format::wav) {
        auto ptr = std::make_unique<codec::wav>(data);
        samples.samples = std::move(ptr->samples());
        samples.channels = ptr->channels();
        samples.bits = ptr->sample_bits();
        samples.rate = ptr->sample_rate();
    }
    return std::move(samples);
}

auto sound::conversion::data(const std::string& result, const sample_information& samples) -> data::block
{
    data::block data;
    if (result == format::snd) {
        auto ptr = std::make_unique<codec::snd>(samples.rate, samples.bits, samples.channels, samples.samples);
        data = std::move(ptr->data());
    }
    else if (result == format::wav) {
        auto ptr = std::make_unique<codec::wav>(samples.rate, samples.bits, samples.channels, samples.samples);
        data = std::move(ptr->data());
    }
    return std::move(data);
}

auto sound::conversion::perform(const std::string& source, const std::string& result, const data::block& data) -> data::block
{
    auto source_surface = conversion::samples(source, data);
    return std::move(conversion::data(result, source_surface));
}