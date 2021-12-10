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

#include <iostream>
#include "core/audio/player/openal_player.hpp"

// MARK: - Template / Helper

template<typename alFunction, typename... Params>
static auto audio_openal_call(const char *filename, const uint_fast32_t line, alFunction fn, Params... params)
-> typename std::enable_if_t<!std::is_same_v<void, decltype(fn(params...))>, decltype(fn(params...))>
{
    auto ret = fn(std::forward<Params>(params)...);
    audio::openal::player::check_errors(filename, line);
    return ret;
}

template<typename alFunction, typename... Params>
static auto audio_openal_call(const char *filename, const uint_fast32_t line, alFunction fn, Params... params)
-> typename std::enable_if_t<std::is_same_v<void, decltype(fn(params...))>, bool>
{
    fn(std::forward<Params>(params)...);
    return audio::openal::player::check_errors(filename, line);
}

template<typename alcFunction, typename... Params>
static auto audio_openal_call(const char *filename, const uint_fast32_t line, alcFunction fn, ALCdevice *dev, Params... params)
-> typename std::enable_if_t<std::is_same_v<void, decltype(fn(params...))>, bool>
{
    fn(std::forward<Params>(params)...);
    return audio::openal::player::check_errors(filename, line, dev);
}

template<typename alcFunction, typename return_type, typename... Params>
static auto audio_openal_call(const char *filename, const uint_fast32_t line, alcFunction fn, return_type& ret, ALCdevice *dev, Params... params)
-> typename std::enable_if_t<!std::is_same_v<void, decltype(fn(params...))>, bool>
{
    ret = fn(std::forward<Params>(params)...);
    return audio::openal::player::check_errors(filename, line, dev);
}


#define openal_do(function, ...) audio_openal_call(__FILE__, __LINE__, function, __VA_ARGS__)
#define openalc_do(function, device, ...) audio_openal_call(__FILE__, __LINE__, function, device, __VA_ARGS__)

// MARK: - Error Checking

auto audio::openal::player::check_errors(const std::string& filename, uint_fast32_t line) -> bool
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")" << std::endl;
        switch (error) {
            case AL_INVALID_NAME:
                std::cerr << "AL_INVALID_NAME: a bad name (id) was passed to an OpenAL function." << std::endl;
                return false;
            case AL_INVALID_ENUM:
                std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function." << std::endl;
                return false;
            case AL_INVALID_VALUE:
                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function." << std::endl;
                return false;
            case AL_INVALID_OPERATION:
                std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid." << std::endl;
                return false;
            case AL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation is not valid." << std::endl;
                return false;
            default:
                std::cerr << "UNKNOWN AL ERROR: " << error << std::endl;
                return false;
        }
    }
    return true;
}


auto audio::openal::player::check_errors(const std::string& filename, uint_fast32_t line, ALCdevice *dev) -> bool
{
    ALCenum error = alcGetError(dev);
    if (error != ALC_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")" << std::endl;
        switch (error) {
            case ALC_INVALID_VALUE:
                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function." << std::endl;
                return false;
            case ALC_INVALID_DEVICE:
                std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function." << std::endl;
                return false;
            case ALC_INVALID_CONTEXT:
                std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function." << std::endl;
                return false;
            case ALC_INVALID_ENUM:
                std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function." << std::endl;
                return false;
            case ALC_OUT_OF_MEMORY:
                std::cerr << "ALC_OUT_OF_MEMORY: the requested operation is not valid." << std::endl;
                return false;
            default:
                std::cerr << "UNKNOWN ALC ERROR: " << error << std::endl;
                return false;
        }
    }
    return true;
}

// MARK: - Configuration

auto audio::openal::player::configure() -> void
{
    m_device = alcOpenDevice(nullptr);
    if (!m_device) {
        m_configured = false;
        return;
    }

    if (!openalc_do(alcCreateContext, m_context, m_device, m_device, nullptr) || !m_context) {
        m_configured = false;
        alcCloseDevice(m_device);
        m_device = nullptr;
        return;
    }

    if (!openalc_do(alcMakeContextCurrent, m_context_current, m_device, m_context) || (m_context_current != ALC_TRUE)) {
        m_configured = false;
        alcDestroyContext(m_context);
        alcCloseDevice(m_device);
        m_context = nullptr;
        m_device = nullptr;
        return;
    }

    m_configured = true;
}

// MARK: - Playback Session

auto audio::openal::player::acquire_player_info() -> playback_session_info
{
    return {};
}

auto audio::openal::player::configure_playback_session(std::shared_ptr<audio::playback_session<playback_session_info>> session) -> void
{
    // Setup the buffer and then the source...
    if (session->item->channels() == 1 && session->item->bit_width() == 8) {
        session->info.format = AL_FORMAT_MONO8;
    }
    else if (session->item->channels() == 1 && session->item->bit_width() == 16) {
        session->info.format = AL_FORMAT_MONO16;
    }
    else if (session->item->channels() == 2 && session->item->bit_width() == 8) {
        session->info.format = AL_FORMAT_STEREO8;
    }
    else if (session->item->channels() == 2 && session->item->bit_width() == 16) {
        session->info.format = AL_FORMAT_STEREO16;
    }
    else {
        return;
    }

    openal_do(alGenBuffers, 1, &session->info.handle);
    openal_do(alBufferData,
              session->info.handle,
              session->info.format,
              session->item->internal_buffer_pointer(),
              session->item->buffer_size(),
              session->item->sample_rate());

    auto err = openal_do(alGenSources, 1, &session->info.source);
    openal_do(alSourcef, session->info.source, AL_PITCH, 1);
    openal_do(alSourcef, session->info.source, AL_GAIN, 1.0f);
    openal_do(alSource3f, session->info.source, AL_POSITION, 0, 0, 0);
    openal_do(alSource3f, session->info.source, AL_VELOCITY, 0, 0, 0);
    openal_do(alSourcei, session->info.source, AL_LOOPING, AL_FALSE);
    openal_do(alSourcei, session->info.source, AL_BUFFER, session->info.handle);
}

// MARK: - Playback Management

auto audio::openal::player::play(std::shared_ptr<audio::player_item> item, std::function<auto()->void> finished) -> playback_session_ref
{
    if (!m_configured) {
        return 0;
    }

    const auto& ref = audio::player<playback_session_info>::play(item, finished);
    const auto& session = audio::player<playback_session_info>::playback_session(ref);

    openal_do(alSourcePlay, session->info.source);

    return ref;
}

auto audio::openal::player::stop(const playback_session_ref& ref) -> void
{
    if (!m_configured) {
        return;
    }

    const auto& session = audio::player<playback_session_info>::playback_session(ref);
    if (session) {
        openal_do(alSourceStop, session->info.source);
        session->ref = 0;
    }
}