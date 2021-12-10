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

#include "core/audio/player/core_audio_player.hpp"

// MARK: - Callbacks
#if __APPLE__

static void core_audio_output_callback(void *userData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
    // Nothing to do here...
}

static void core_audio_playback_completion_callback(void *inUserData, AudioQueueRef inAQ, AudioQueuePropertyID inID)
{
//    auto playback_ref = reinterpret_cast<audio::manager::playback_reference *>(inUserData);
//
//    uint32_t is_running = 0;
//    uint32_t property_size = 0;
//    AudioQueueGetPropertySize(inAQ, kAudioQueueProperty_IsRunning, &property_size);
//    AudioQueueGetProperty(inAQ, kAudioQueueProperty_IsRunning, &is_running, &property_size);
//
//    if (is_running == 0) {
//        audio::manager::shared_manager().finish_playback(playback_ref->id);
//    }
}

#endif

// MARK: - Playback Session

auto audio::core_audio::player::acquire_player_info() -> audio::core_audio::playback_session_info
{
    return {};
}

auto audio::core_audio::player::configure_playback_session(std::shared_ptr<audio::playback_session<playback_session_info>> session) -> void
{
#if __APPLE__
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
    AudioQueueNewOutput(&session->info.stream_desc, core_audio_output_callback, nullptr, nullptr, nullptr, 0, &session->info.queue);
    AudioQueueAllocateBuffer(session->info.queue, session->item->buffer_size(), &session->info.buffer);
    memcpy(session->info.buffer->mAudioData, session->item->internal_buffer_pointer(), session->item->buffer_size());
    session->info.buffer->mAudioDataByteSize = session->item->buffer_size();

    // Setup the playback completion listener
    AudioQueueAddPropertyListener(session->info.queue, kAudioQueueProperty_IsRunning, core_audio_playback_completion_callback, reinterpret_cast<void *>(session.get()));
#endif
}
// MARK: - Playback

auto audio::core_audio::player::play(std::shared_ptr<audio::player_item> item, std::function<auto()->void> finished) -> audio::playback_session_ref
{
#if __APPLE__
    const auto& ref = audio::player<playback_session_info>::play(item, finished);
    const auto& session = audio::player<playback_session_info>::playback_session(ref);

    // Invoke the audio within Core Audio. We need to setup the finishing callback to notify the engine that audio
    // playback has completed.
    AudioQueueReset(session->info.queue);
    AudioQueueEnqueueBuffer(session->info.queue, session->info.buffer, 0, nullptr);
    AudioQueueStart(session->info.queue, nullptr);
    AudioQueueStop(session->info.queue, false);

    return ref;
#else
    return 0;
#endif
}

auto audio::core_audio::player::stop(const audio::playback_session_ref &ref) -> void
{
#if __APPLE__
    const auto& session = audio::player<playback_session_info>::playback_session(ref);
    if (session != nullptr) {
        AudioQueueStop(session->info.queue, true);
    }
#endif
}
