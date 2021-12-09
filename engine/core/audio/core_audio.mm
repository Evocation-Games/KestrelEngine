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

#if __APPLE__

#include <thread>
#include <AudioToolbox/AudioToolbox.h>
#include "core/audio/core_audio.hpp"
#include "core/support/macos/cocoa/cocoa_utils.h"

static void audioManagerSimpleSoundOutputCallback(void *userData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{

}

static void coreAudioPlaybackCompletionCallback(void *inUserData, AudioQueueRef inAQ, AudioQueuePropertyID inID)
{
    auto playback_ref = reinterpret_cast<audio::manager::playback_reference *>(inUserData);

    uint32_t is_running = 0;
    uint32_t property_size = 0;
    AudioQueueGetPropertySize(inAQ, kAudioQueueProperty_IsRunning, &property_size);
    AudioQueueGetProperty(inAQ, kAudioQueueProperty_IsRunning, &is_running, &property_size);

    if (is_running == 0) {
        audio::manager::shared_manager().finish_playback(playback_ref->id);
    }
}

auto audio::core_audio::play(audio::manager::playback_reference *ref) -> void
{
    if (ref == nullptr) {
        return;
    }

    const auto& chunk = ref->chunk;

    // Create a simple stream description
    AudioStreamBasicDescription streamDesc = { 0 };
    streamDesc.mSampleRate = chunk->sample_rate;
    streamDesc.mChannelsPerFrame = chunk->channels;
    streamDesc.mFormatID = chunk->format_id;
    streamDesc.mFormatFlags = chunk->format_flags;
    streamDesc.mBitsPerChannel = chunk->bit_width;
    streamDesc.mBytesPerFrame = chunk->bytes_per_frame;
    streamDesc.mFramesPerPacket = chunk->frames_per_packet;
    streamDesc.mBytesPerPacket = chunk->bytes_per_packet;

    // Setup Audio Queue
    AudioQueueRef queue;
    AudioQueueBufferRef buffer;
    AudioQueueNewOutput(&streamDesc, audioManagerSimpleSoundOutputCallback, nullptr, nullptr, nullptr, 0, &queue);
    AudioQueueAllocateBuffer(queue, chunk->data_size, &buffer);
    memcpy(buffer->mAudioData, chunk->internal_data_pointer(), chunk->data_size);
    buffer->mAudioDataByteSize = chunk->data_size;

    AudioQueueAddPropertyListener(queue, kAudioQueueProperty_IsRunning, coreAudioPlaybackCompletionCallback, reinterpret_cast<void *>(ref));

    AudioQueueReset(queue);
    AudioQueueEnqueueBuffer(queue, buffer, 0, nullptr);
    AudioQueueStart(queue, nullptr);
    AudioQueueStop(queue, false);
}

// MARK: - Background Audio

#include <AVFoundation/AVFoundation.h>

static AVAudioPlayer *backgroundAudioPlayer = nil;

auto audio::core_audio::play_background_audio(const std::string& path) -> void
{
    if (backgroundAudioPlayer) {
        [backgroundAudioPlayer stop];
        [backgroundAudioPlayer release];
        backgroundAudioPlayer = nil;
    }

    NSError *err = nil;
    NSURL *audioFileURL = [NSURL fileURLWithPath:cocoa::string::to(path)];
    backgroundAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:audioFileURL error:&err];
    if (!backgroundAudioPlayer || err) {
        [backgroundAudioPlayer release];
        backgroundAudioPlayer = nil;
    }

    [backgroundAudioPlayer play];
}

auto audio::core_audio::stop_background_audio() -> void
{
    if (backgroundAudioPlayer) {
        [backgroundAudioPlayer stop];
        [backgroundAudioPlayer release];
        backgroundAudioPlayer = nil;
    }
}

#endif