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

#include <libKestrel/util/availability.hpp>

#if TARGET_MACOS

#include <AVFoundation/AVFoundation.h>
#include <libKestrel/platform/macos/cocoa_utils.h>
#include <libKestrel/sound/player/core_audio_player.hpp>
#include <libKestrel/sound/audio_manager.hpp>

// MARK: - Callback

static void core_audio_output_callback(void *userData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
    // Nothing to do here...
}

static void core_audio_playback_completion_callback(void *inUserData, AudioQueueRef inAQ, AudioQueuePropertyID inID)
{
    auto session = reinterpret_cast<kestrel::sound::playback_session<kestrel::sound::core_audio::playback_session_info>*>(inUserData);

    std::uint32_t is_running = 0;
    std::uint32_t property_size = 0;
    AudioQueueGetPropertySize(inAQ, kAudioQueueProperty_IsRunning, &property_size);
    AudioQueueGetProperty(inAQ, kAudioQueueProperty_IsRunning, &is_running, &property_size);

    if (is_running == 0) {
        session->finish();
        kestrel::sound::manager::shared_manager().finish_item(session->ref);
    }
}

#endif

// MARK: - Playback Session

auto kestrel::sound::core_audio::player::acquire_player_info() -> kestrel::sound::core_audio::playback_session_info
{
    return {};
}

auto kestrel::sound::core_audio::player::configure_playback_session(std::shared_ptr<sound::playback_session<playback_session_info>> session) -> void
{
    if (session->item->is_file_item()) {
        session->info.item = [[AVPlayerItem alloc] initWithURL:[NSURL fileURLWithPath:platform::macos::cocoa::string::to(session->item->file_path())]];
    }
    else {
        // Configure the stream descriptor
        session->info.stream_desc = { 0 };
        session->info.stream_desc.mSampleRate = session->item->sample_rate();
        session->info.stream_desc.mChannelsPerFrame = session->item->channels();
        session->info.stream_desc.mFormatID = session->item->format();
        session->info.stream_desc.mFormatFlags = session->item->format_flags();
        session->info.stream_desc.mBitsPerChannel = session->item->bit_width();
        session->info.stream_desc.mBytesPerFrame = session->item->bytes_per_frame();
        session->info.stream_desc.mFramesPerPacket = session->item->frames_per_packet();
        session->info.stream_desc.mBytesPerPacket = session->item->bytes_per_packet();

        // Establish the audio queue for playback
        AudioQueueRef queue;
        AudioQueueBufferRef buffer;
        AudioQueueNewOutput(&session->info.stream_desc, core_audio_output_callback, nullptr, nullptr, nullptr, 0, &queue);
        AudioQueueAllocateBuffer(queue, session->item->buffer_size(), &buffer);
        memcpy(buffer->mAudioData, session->item->internal_buffer_pointer(), session->item->buffer_size());
        buffer->mAudioDataByteSize = session->item->buffer_size();

        session->info.buffer = buffer;
        session->info.queue = queue;

        // Setup the playback completion listener
        auto data = session.get();
        AudioQueueAddPropertyListener(session->info.queue, kAudioQueueProperty_IsRunning, core_audio_playback_completion_callback, reinterpret_cast<void *>(data));
    }
}
// MARK: - Playback

auto kestrel::sound::core_audio::player::play(std::shared_ptr<player_item> item, std::function<auto()->void> finished) -> playback_session_ref
{
    const auto& ref = sound::player<playback_session_info>::play(item, finished);
    const auto& session = sound::player<playback_session_info>::playback_session(ref);

    // Invoke the audio within Core Audio. We need to setup the finishing callback to notify the engine that audio
    // playback has completed.
    if (session->item->is_file_item()) {
        if (!m_player) {
            m_player = [[AVPlayer alloc] initWithPlayerItem:session->info.item];
        }
        else {
            [m_player replaceCurrentItemWithPlayerItem:session->info.item];
        }
        [m_player play];
    }
    else {
        AudioQueueReset(session->info.queue);
        AudioQueueEnqueueBuffer(session->info.queue, session->info.buffer, 0, nullptr);
        AudioQueueSetParameter(session->info.queue, kAudioQueueParam_Volume, 0.15);
        AudioQueueStart(session->info.queue, nullptr);
        AudioQueueStop(session->info.queue, false);
    }

    return ref;
}

auto kestrel::sound::core_audio::player::stop(const sound::playback_session_ref &ref) -> void
{
    const auto& session = sound::player<playback_session_info>::playback_session(ref);
    if (session != nullptr) {
        if (session->item->is_file_item()) {
            if (m_player) {
                [m_player pause];
                m_player = nullptr;
            }
        }
        else {
            AudioQueueStop(session->info.queue, true);
            session->ref = 0;
        }
    }
}
