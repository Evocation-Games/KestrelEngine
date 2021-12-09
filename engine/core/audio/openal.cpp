//
// Created by tomhancocks on 19/11/2021.
//

#if !__APPLE__

#include <iostream>
#include "core/audio/openal.hpp"

#define MINIMP3_ONLY_MP3
#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

// MARK : - Singleton

auto audio::openal::player::global() -> player &
{
    static audio::openal::player instance;
    return instance;
}

audio::openal::player::player()
{
    m_playback_thread = std::make_unique<std::thread>([&] {
        this->playback_loop();
    });
}

// MARK: - Templates / Helpers

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

// MARK: - Configuration

auto audio::openal::player::configure_devices() -> bool
{
    if (m_configuration_attempted) {
        return m_configured;
    }

    m_configuration_attempted = true;

    m_device = alcOpenDevice(nullptr);
    if (!m_device) {
        m_configured = false;
        return m_configured;
    }

    if (!openalc_do(alcCreateContext, m_context, m_device, m_device, nullptr) || !m_context) {
        m_configured = false;
        alcCloseDevice(m_device);
        m_device = nullptr;
        return m_configured;
    }

    if (!openalc_do(alcMakeContextCurrent, m_context_current, m_device, m_context) || (m_context_current != ALC_TRUE)) {
        m_configured = false;
        alcDestroyContext(m_context);
        alcCloseDevice(m_device);
        m_context = nullptr;
        m_device = nullptr;
        return m_configured;
    }

    return m_configured;
}

// MARK: - Error

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

// MARK: - Playback

auto audio::openal::player::play(std::shared_ptr<audio::chunk> chunk) -> bool
{
    auto pb = std::make_unique<playback_buffer>();
    pb->chunk = chunk;

    // Setup the buffer and then the source...
    if (chunk->channels == 1 && chunk->bit_width == 8) {
        pb->format = AL_FORMAT_MONO8;
    }
    else if (chunk->channels == 1 && chunk->bit_width == 16) {
        pb->format = AL_FORMAT_MONO16;
    }
    else if (chunk->channels == 2 && chunk->bit_width == 8) {
        pb->format = AL_FORMAT_STEREO8;
    }
    else if (chunk->channels == 2 && chunk->bit_width == 16) {
        pb->format = AL_FORMAT_STEREO16;
    }
    else {
        return false;
    }

    openal_do(alGenBuffers, 1, &pb->handle);
    openal_do(alBufferData, pb->handle, pb->format, chunk->internal_data_pointer(), chunk->data_size, chunk->sample_rate);

    ALuint source;
    auto err = openal_do(alGenSources, 1, &source);
    pb->source = source;
    openal_do(alSourcef, pb->source, AL_PITCH, 1);
    openal_do(alSourcef, pb->source, AL_GAIN, 1.0f);
    openal_do(alSource3f, pb->source, AL_POSITION, 0, 0, 0);
    openal_do(alSource3f, pb->source, AL_VELOCITY, 0, 0, 0);
    openal_do(alSourcei, pb->source, AL_LOOPING, AL_FALSE);
    openal_do(alSourcei, pb->source, AL_BUFFER, pb->handle);

    // play the audio.
    openal_do(alSourcePlay, pb->source);

    // inject the playback packet into the queue.
    m_buffers.emplace_back(std::move(pb));

    return true;
}

auto audio::openal::player::playback_loop() -> void
{

}

// MARK: - Audio Files / MP3

auto audio::openal::player::play_background_audio(const std::string& path) -> void
{
    // TODO: Make sure there isn't already a background audio task running...

    mp3dec_t mp3d;
    mp3dec_file_info_t info;
    if (mp3dec_load(&mp3d, path.c_str(), &info, nullptr, nullptr)) {
        // TODO: Handle error case...
        return;
    }

    auto chunk = std::make_shared<audio::chunk>();
    chunk->channels = info.channels;
    chunk->bit_width = 16;
    chunk->data_size = info.samples;
    chunk->data = info.buffer;
    chunk->sample_rate = info.hz;
    play(chunk);
}

auto audio::openal::player::stop_background_audio() -> void
{

}

#endif