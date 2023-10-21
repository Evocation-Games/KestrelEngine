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

#include <libKestrel/platform/windows/fonts.hpp>

#if TARGET_WINDOWS
#include <windows.h>
#include <sstream>

auto kestrel::platform::windows::font::system_font() -> std::string
{
    return path_for("Arial");
}

auto kestrel::platform::windows::font::path_for(const std::string& name) -> std::string
{
    // TODO: This is probably not correct/optimal, but will need to be addressed in the future.
    char text[] = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
    wchar_t wtext[51];
    std::mbstowcs(wtext, text, strlen(text) + 1);
    LPWSTR font_registry_path = wtext;

    HKEY hkey;
    LONG result;
    std::wstring ws_face_name(name.begin(), name.end());

    // Open the Windows font registry.
    result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, font_registry_path, 0, KEY_READ, &hkey);
    if (result != ERROR_SUCCESS) {
        return "";
    }

    DWORD max_value_name_size, max_value_data_size;
    result = RegQueryInfoKey(hkey, 0, 0, 0, 0, 0, 0, 0, &max_value_name_size, &max_value_data_size, 0, 0);
    if (result != ERROR_SUCCESS) {
        return "";
    }

    DWORD value_index = 0;
    LPWSTR value_name = new WCHAR[max_value_data_size];
    LPBYTE value_data = new BYTE[max_value_data_size];
    DWORD value_name_size, value_data_size, value_type;
    std::wstring ws_font_file;

    // Look for a matching font file.
    do {
        ws_font_file.clear();
        value_data_size = max_value_data_size;
        value_name_size = max_value_name_size;

        result = RegEnumValueW(hkey, value_index, value_name, &value_name_size, 0, &value_type, value_data, &value_data_size);
        value_index++;

        if (result != ERROR_SUCCESS || value_type != REG_SZ) {
            continue;
        }

        std::wstring ws_value_name(value_name, value_name_size);

        // Check to see if we found a match?
        if (_wcsnicmp(ws_face_name.c_str(), ws_value_name.c_str(), ws_face_name.length()) == 0) {
            ws_font_file.assign((LPWSTR)value_data, value_data_size);
            break;
        }
    }
    while (result != ERROR_NO_MORE_ITEMS);

    delete[] value_name;
    delete[] value_data;

    RegCloseKey(hkey);

    if (ws_font_file.empty()) {
        return "";
    }

    // Build full font file path.
    WCHAR windows_directory[MAX_PATH];
    GetWindowsDirectoryW(windows_directory, MAX_PATH);

    std::wstringstream ss;
    ss << windows_directory << "\\Fonts\\" << ws_font_file;
    ws_font_file = ss.str();

    return { ws_font_file.begin(), ws_font_file.end() };
}

#else
empty_file_stub;
#endif
