//
// Created by Tom Hancocks on 03/04/2020.
//

#include <sys/stat.h>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <utility>
#include <vector>
#include <functional>
#include <cstring>

#include <libFoundation/availability.hpp>
#include <libFoundation/system/filesystem/file.hpp>

// MARK: - Constructors

foundation::filesystem::file::file(filesystem::path path, file_mode mode)
    : m_path(std::move(path))
{
    if (m_path.exists()) {
        std::ifstream f(m_path.string(), std::ios::binary);

        // Reserve space in the m_contents string, equivalent to the length of the file.
        f.seekg(0, std::ios::end);
        m_length = f.tellg();
        m_raw = new uint8_t[m_length + 1];
        memset(m_raw, 0, m_length);
        f.seekg(0, std::ios::beg);

        // Read in the contents of the file.
        f.read((char *)m_raw, m_length);
        m_raw[m_length++] = '\n';

        f.close();
    }
    else {
        m_raw = nullptr;
    }
}

foundation::filesystem::file::file(const std::string &name, const std::string &contents)
    : m_path(name)
{
    m_length = contents.size();
    m_raw = new uint8_t[m_length + 1];
    memset(m_raw, 0, m_length);
    memcpy(m_raw, contents.c_str(), m_length);
}

// MARK: - Destructors

foundation::filesystem::file::~file()
{
    delete[] m_raw;
}

// MARK: - Accessors

auto foundation::filesystem::file::exists() const -> bool
{
    return m_path.exists() && m_length > 0 && m_raw != nullptr;
}

auto foundation::filesystem::file::path() const -> filesystem::path
{
    return m_path;
}

auto foundation::filesystem::file::string_contents() const -> const std::string&
{
    if (m_cache.string_contents.empty()) {
        const_cast<file *>(this)->m_cache.string_contents.assign(m_raw, m_raw + m_length);
    }
    return m_cache.string_contents;
}

auto foundation::filesystem::file::characters() const -> std::vector<char>
{
    std::vector<char> v(m_raw, m_raw + m_length);
    return std::move(v);
}

auto foundation::filesystem::file::set_string_contents(const std::string& contents) -> void
{
    delete[] m_raw;
    m_length = static_cast<decltype(m_length)>(contents.size());
    m_raw = new uint8_t[m_length];
    memcpy(m_raw, contents.c_str(), m_length);
}

auto foundation::filesystem::file::set_characters(const std::vector<char>& contents) -> void
{

}

auto foundation::filesystem::file::set_bytes(const std::vector<std::uint8_t>& contents) -> void
{

}

auto foundation::filesystem::file::bytes() const -> std::vector<std::uint8_t>
{
    std::vector<std::uint8_t> v(m_raw, m_raw + m_length);
    return std::move(v);
}

// MARK: - Saving

auto foundation::filesystem::file::save(const filesystem::path& path) -> void
{
    // TODO: This needs to be a binary file save rather than text based.
    if (path.empty()) {
        return;
    }
    m_path = path;
    std::ofstream out(m_path.string());
    out << string_contents();
    out.close();
}
