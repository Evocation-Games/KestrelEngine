//
// Created by Tom Hancocks on 13/11/2021.
//

#include <thread>
#include "core/audio/audio_manager.hpp"

// MARK: - Manager

auto audio::manager::shared_manager() -> manager &
{
    static audio::manager instance;
    return instance;
}

// MARK: - Configuration

#if __APPLE__

auto audio::manager::configure() -> void
{

}

#else

#include "core/audio/openal.hpp"

auto audio::manager::configure() -> void
{
    openal::player::global().configure_devices();
}

#endif

// MARK: - Audio Playback

auto audio::manager::play(audio::sound snd) -> void
{
    openal::player::global().play(audio::sound(snd));
}

#if __APPLE__

//#include <AudioToolbox/AudioToolbox.h>
//
//static void audioManagerSimpleSoundOutputCallback(void *userData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
//{
//
//}
//
//auto audio::play_sound(const asset::macintosh_sound& snd) -> void
//{
//    std::thread t([&snd] {
//        // Create a simple stream description
//        AudioStreamBasicDescription streamDesc = { 0 };
//        streamDesc.mSampleRate = snd.sample_rate();
//        streamDesc.mChannelsPerFrame = snd.channels();
//        streamDesc.mFormatID = snd.format();
//        streamDesc.mFormatFlags = snd.format_flags();
//        streamDesc.mBitsPerChannel = snd.bits_per_channel();
//        streamDesc.mBytesPerFrame = snd.bytes_per_frame();
//        streamDesc.mFramesPerPacket = snd.frames_per_packet();
//        streamDesc.mBytesPerPacket = snd.bytes_per_packet();
//
//        // Setup Audio Queue
//        AudioQueueRef queue;
//        AudioQueueBufferRef buffer;
//        uint32_t byteSize = snd.raw_data_size();
//        AudioQueueNewOutput(&streamDesc, audioManagerSimpleSoundOutputCallback, nullptr, nullptr, nullptr, 0, &queue);
//        AudioQueueAllocateBuffer(queue, byteSize, &buffer);
//        memcpy(buffer->mAudioData, snd.raw_data(), byteSize);
//        buffer->mAudioDataByteSize = byteSize;
//
//        AudioQueueReset(queue);
//        AudioQueueEnqueueBuffer(queue, buffer, 0, nullptr);
//        AudioQueueStart(queue, nullptr);
//        AudioQueueStop(queue, false);
//    });
//    t.join();
//}
//
//#else
//
//#include <AL/al.h>
//#include <AL/alc.h>
//
//static auto audio_check_errors(const std::string& filename, const std::uint_fast32_t line) -> bool
//{
//    ALenum error = alGetError();
//    if (error != AL_NO_ERROR) {
//        std::cerr << "***ERROR*** (" << filename << ": " << line << ")" << std::endl;
//        switch (error) {
//            case AL_INVALID_NAME:
//                std::cerr << "AL_INVALID_NAME: a bad name (id) was passed to an OpenAL function." << std::endl;
//                return false;
//            case AL_INVALID_ENUM:
//                std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function." << std::endl;
//                return false;
//            case AL_INVALID_VALUE:
//                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function." << std::endl;
//                return false;
//            case AL_INVALID_OPERATION:
//                std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid." << std::endl;
//                return false;
//            case AL_OUT_OF_MEMORY:
//                std::cerr << "AL_OUT_OF_MEMORY: the requested operation is not valid." << std::endl;
//                return false;
//            default:
//                std::cerr << "UNKNOWN AL ERROR: " << error << std::endl;
//                return false;
//        }
//    }
//    return true;
//}
//
//
//static auto audio_check_errors_alc(const std::string& filename, const std::uint_fast32_t line, ALCdevice *dev) -> bool
//{
//    ALCenum error = alcGetError(dev);
//    if (error != ALC_NO_ERROR) {
//        std::cerr << "***ERROR*** (" << filename << ": " << line << ")" << std::endl;
//        switch (error) {
//            case ALC_INVALID_VALUE:
//                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function." << std::endl;
//                return false;
//            case ALC_INVALID_DEVICE:
//                std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function." << std::endl;
//                return false;
//            case ALC_INVALID_CONTEXT:
//                std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function." << std::endl;
//                return false;
//            case ALC_INVALID_ENUM:
//                std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function." << std::endl;
//                return false;
//            case ALC_OUT_OF_MEMORY:
//                std::cerr << "ALC_OUT_OF_MEMORY: the requested operation is not valid." << std::endl;
//                return false;
//            default:
//                std::cerr << "UNKNOWN ALC ERROR: " << error << std::endl;
//                return false;
//        }
//    }
//    return true;
//}
//
//template<typename alFunction, typename... Params>
//static auto al_call_impl(const char *filename, const uint_fast32_t line, alFunction fn, Params... params)
//    -> typename std::enable_if_t<!std::is_same_v<void, decltype(fn(params...))>, decltype(fn(params...))>
//{
//    auto ret = fn(std::forward<Params>(params)...);
//    audio_check_errors(filename, line);
//    return ret;
//}
//
//template<typename alFunction, typename... Params>
//static auto al_call_impl(const char *filename, const uint_fast32_t line, alFunction fn, Params... params)
//    -> typename std::enable_if_t<std::is_same_v<void, decltype(fn(params...))>, bool>
//{
//    fn(std::forward<Params>(params)...);
//    return audio_check_errors(filename, line);
//}
//
//template<typename alcFunction, typename... Params>
//static auto alc_call_impl(const char *filename, const uint_fast32_t line, alcFunction fn, ALCdevice *dev, Params... params)
//    -> typename std::enable_if_t<std::is_same_v<void, decltype(fn(params...))>, bool>
//{
//    fn(std::forward<Params>(params)...);
//    return audio_check_errors_alc(filename, line, dev);
//}
//
//template<typename alcFunction, typename return_type, typename... Params>
//static auto alc_call_impl(const char *filename, const uint_fast32_t line, alcFunction fn, return_type& ret, ALCdevice *dev, Params... params)
//    -> typename std::enable_if_t<!std::is_same_v<void, decltype(fn(params...))>, bool>
//{
//    ret = fn(std::forward<Params>(params)...);
//    return audio_check_errors_alc(filename, line, dev);
//}
//
//#define alCall(function, ...) al_call_impl(__FILE__, __LINE__, function, __VA_ARGS__)
//#define alcCall(function, device, ...) alc_call_impl(__FILE__, __LINE__, function, device, __VA_ARGS__)
//
//auto audio::play_sound(const asset::macintosh_sound &snd) -> void
//{
//    std::thread sound_thread([snd] {
//
//        ALCdevice *dev = alcOpenDevice(nullptr);
//        if (!dev) {
//            return;
//        }
//
//        ALCcontext *ctx;
//        if (!alcCall(alcCreateContext, ctx, dev, dev, nullptr) || !ctx) {
//            return;
//        }
//
//        ALCboolean ctx_made_current = false;
//        if (!alcCall(alcMakeContextCurrent, ctx_made_current, dev, ctx) || ctx_made_current != ALC_TRUE) {
//            return;
//        }
//
//        ALuint buffer;
//        alCall(alGenBuffers, 1, &buffer);
//
//        ALenum format;
//        if (snd.channels() == 1 && snd.bits_per_channel() == 8) {
//            format = AL_FORMAT_MONO8;
//        }
//        else if (snd.channels() == 1 && snd.bits_per_channel() == 16) {
//            format = AL_FORMAT_MONO16;
//        }
//        else if (snd.channels() == 2 && snd.bits_per_channel() == 8) {
//            format = AL_FORMAT_STEREO8;
//        }
//        else if (snd.channels() == 2 && snd.bits_per_channel() == 16) {
//            format = AL_FORMAT_STEREO16;
//        }
//        else {
//            return;
//        }
//
//        alCall(alBufferData, buffer, format, snd.raw_data(), snd.raw_data_size(), snd.sample_rate());
//
//        ALuint source;
//        alCall(alGenSources, 1, &source);
//        alCall(alSourcef, source, AL_PITCH, 1);
//        alCall(alSourcef, source, AL_GAIN, 1.0f);
//        alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
//        alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
//        alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
//        alCall(alSourcei, source, AL_BUFFER, buffer);
//
//        alCall(alSourcePlay, source);
//
//        ALint state = AL_PLAYING;
//
//        while(state == AL_PLAYING)
//        {
//            alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
//        }
//
//        alCall(alDeleteSources, 1, &source);
//        alCall(alDeleteBuffers, 1, &buffer);
//
//        alcCall(alcMakeContextCurrent, ctx_made_current, dev, nullptr);
//        alcCall(alcDestroyContext, dev, ctx);
//
//        ALCboolean closed;
//        alcCall(alcCloseDevice, closed, dev, dev);
//    });
//    sound_thread.join();
//}

#endif
