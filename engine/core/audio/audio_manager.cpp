//
// Created by Tom Hancocks on 13/11/2021.
//

#include <thread>
#include "core/audio/audio_manager.hpp"

// MARK: - Audio Playback

#if __APPLE__

#include <AudioToolbox/AudioToolbox.h>

static void audioManagerSimpleSoundOutputCallback(void *userData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{

}

auto audio::play_sound(const asset::macintosh_sound& snd) -> void
{
    std::thread t([&snd] {
        // Create a simple stream description
        AudioStreamBasicDescription streamDesc = { 0 };
        streamDesc.mSampleRate = snd.sample_rate();
        streamDesc.mChannelsPerFrame = snd.channels();
        streamDesc.mFormatID = snd.format();
        streamDesc.mFormatFlags = snd.format_flags();
        streamDesc.mBitsPerChannel = snd.bits_per_channel();
        streamDesc.mBytesPerFrame = snd.bytes_per_frame();
        streamDesc.mFramesPerPacket = snd.frames_per_packet();
        streamDesc.mBytesPerPacket = snd.bytes_per_packet();

        // Setup Audio Queue
        AudioQueueRef queue;
        AudioQueueBufferRef buffer;
        uint32_t byteSize = snd.raw_data_size();
        AudioQueueNewOutput(&streamDesc, audioManagerSimpleSoundOutputCallback, nullptr, nullptr, nullptr, 0, &queue);
        AudioQueueAllocateBuffer(queue, byteSize, &buffer);
        memcpy(buffer->mAudioData, snd.raw_data(), byteSize);
        buffer->mAudioDataByteSize = byteSize;

        AudioQueueReset(queue);
        AudioQueueEnqueueBuffer(queue, buffer, 0, nullptr);
        AudioQueueStart(queue, nullptr);
        AudioQueueStop(queue, false);
    });
    t.join();
}

#endif
