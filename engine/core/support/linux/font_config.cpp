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

#if __linux__

#include <fontconfig/fontconfig.h>
#include <stdexcept>
#include "font_config.hpp"

static FcConfig *config = NULL;

auto linux_os::font_config::path_for_best_fit_font(const std::string &font) -> std::string
{
    // Check if we need to load and configure the font config before proceeding.
    if (!config) {
        config = FcInitLoadConfigAndFonts();
    }

    // Build a pattern from the requested font name.
    auto pattern = FcNameParse(reinterpret_cast<const FcChar8*>(font.c_str()));
    FcConfigSubstitute(config, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    std::string font_file;
    FcResult result;
    auto font_ref = FcFontMatch(config, pattern, &result);
    if (font_ref) {
        FcChar8* path = NULL;
        if (FcPatternGetString(font_ref, FC_FILE, 0, &path) == FcResultMatch) {
            font_file = std::string(reinterpret_cast<const char *>(path));
        }
    }
    else {
        // We could not find a suitable font! Error time.
        throw std::logic_error("Could not locate a suitable font file for '" + font + "'");
    }

    FcPatternDestroy(pattern);
    return font_file;
}


#else
// Stub to prevent empty file warnings
static void empty_stub() {}
#endif
