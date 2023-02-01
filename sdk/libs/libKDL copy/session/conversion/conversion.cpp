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

#include <libKDL/session/conversion/conversion.hpp>
#include <libKDL/exception/incompatible_conversion_exception.hpp>
#include <libKDL/exception/unknown_binary_conversion_format_exception.hpp>

// MARK: - Construction

kdl::session::conversion::conversion(const lexer::lexeme &input, const lexer::lexeme &output)
    : m_input(input), m_output(output)
{
    if (!validate_format(input)) {
        throw unknown_binary_conversion_format_exception("Unknown conversion source format encountered '" + input.text() + "'", input);
    }

    if (!validate_format(output)) {
        throw unknown_binary_conversion_format_exception("Unknown conversion destination format encountered '" + output.text() + "'", output);
    }

    if (!is_convertible(input, output)) {
        throw incompatible_conversion_exception("Incompatible conversion source and destination formats encountered. '" + input.text() + "' -> '" + output.text() + "'.", input, output);
    }
}

// MARK: - Accessors

auto kdl::session::conversion::input_format() const -> lexer::lexeme
{
    return m_input;
}

auto kdl::session::conversion::output_format() const -> lexer::lexeme
{
    return m_output;
}

// MARK: - Validators

auto kdl::session::conversion::validate_format(const lexer::lexeme& type) -> bool
{
    return type.is("PICT")
        || type.is("MacintoshPicture")
        || type.is("TGA")
        || type.is("PNG")
        || type.is("cicn")
        || type.is("ColorIcon")
        || type.is("rleD")
        || type.is("rleX")
        || type.is("ppat")
        || type.is("PixelPattern")
        || type.is("WAV")
        || type.is("WaveAudio")
        || type.is("snd")
        || type.is("MacintoshSound")
        || type.is("MP3");
}

auto kdl::session::conversion::is_convertible(const lexer::lexeme& input, const lexer::lexeme& output) -> bool
{
    if (input.is("PICT") || input.is("MacintoshPicture")) {
        return output.is("TGA")
            || output.is("PNG")
            || output.is("cicn")
            || output.is("ColorIcon")
            || output.is("ppat")
            || output.is("PixelPattern")
            || output.is("rleD")
            || output.is("rleX");
    }
    else if (input.is("TGA")) {
        return output.is("PICT")
            || output.is("MacintoshPicture")
            || output.is("PNG")
            || output.is("cicn")
            || output.is("ColorIcon")
            || output.is("ppat")
            || output.is("PixelPattern")
            || output.is("rleD")
            || output.is("rleX");
    }
    else if (input.is("PNG")) {
        return output.is("PICT")
            || output.is("MacintoshPicture")
            || output.is("TGA")
            || output.is("cicn")
            || output.is("ColorIcon")
            || output.is("ppat")
            || output.is("PixelPattern")
            || output.is("rleD")
            || output.is("rleX");
    }
    else if (input.is("cicn") || input.is("ColorIcon")) {
        return output.is("TGA")
            || output.is("PNG")
            || output.is("PICT")
            || output.is("MacintoshPicture")
            || output.is("ppat")
            || output.is("PixelPattern")
            || output.is("rleD")
            || output.is("rleX");
    }
    else if (input.is("ppat") || input.is("PixelPattern")) {
        return output.is("TGA")
            || output.is("PNG")
            || output.is("PICT")
            || output.is("MacintoshPicture")
            || output.is("cicn")
            || output.is("ColorIcon")
            || output.is("rleD")
            || output.is("rleX");
    }
    else if (input.is("rleD")) {
        return output.is("PNG")
            || output.is("TGA");
    }
    else if (input.is("rleX")) {
        return output.is("PNG")
            || output.is("TGA");
    }
    else if (input.is("WAV") || input.is("WaveAudio")) {
        return output.is("snd")
            || output.is("MacintoshSound")
            || output.is("MP3");
    }
    else if (input.is("snd") || input.is("MacintoshSound")) {
        return output.is("WAV")
            || output.is("WaveAudio")
            || output.is("MP3");
    }
    else if (input.is("MP3")) {
        return output.is("WAV")
            || output.is("WaveAudio")
            || output.is("snd")
            || output.is("MacintoshSound");
    }
    return false;
}
