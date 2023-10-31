// Copyright (c) 2023 Tom Hancocks
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

#include <thread>
#include <libRenderCore/driver/driver.hpp>
#include <libFoundation/availability.hpp>

#if TARGET_MACOS
#   include <libMetalRenderer/driver/driver.hpp>
#endif

#include <libOpenGLRenderer/driver/driver.hpp>

// MARK: - Construction

renderer::driver::driver()
{
    initialize_backend_driver();
    m_api.bindings.core.initialize();

    // Hook up any events that need to act upon the bindings.
    m_store.texture.when_texture_removed([&] (texture::device_id id) {
        m_api.bindings.texture.destroy(id);
    });
}

// MARK: - Interface

auto renderer::driver::start(frame_request_callback request) -> void
{
    // Create the frame object that will be responsible for handling draw operations.
    m_render.frame = renderer::frame(this);

    // Kick off the driver, so that it can start requesting frames
    start_driver(std::move(request));
}

// MARK: - Backend Drivers

auto renderer::driver::initialize_backend_driver() -> void
{
    // We need to determine the order of driver preference, as we need to be sure
    // that we do not attempt to load a driver that is not compatible.
    std::vector<display_configuration::graphics_layer_preference> supported;
    auto default_preference = display_configuration::OPENGL;

    // Uncomment libraries as they are implemented and supported.
#if TARGET_MACOS
    if (metal::driver::api_info().supported()) {
        supported.emplace_back(display_configuration::METAL);
        default_preference = display_configuration::METAL;
    }
#elif TARGET_LINUX
//    supported.emplace_back(DRIVER_VULKAN);
#elif TARGET_WINDOWS
//    supported.emplace_back(DRIVER_VULKAN);
//    supported.emplace_back(DRIVER_DIRECTX);
#endif
    supported.emplace_back(display_configuration::OPENGL);

    // Check the preference chosen by the user, and determine if it is in the list of
    // currently available drivers. If it isn't, then use the default (first).
    auto preference = (m_config.display.gl_preference() == display_configuration::DEFAULT) ? default_preference : m_config.display.gl_preference();
    if (std::find(supported.begin(), supported.end(), preference) == supported.end()) {
        // The library is not supported, so choose the default.
        preference = supported.front();
    }

    // Load the backend driver.
    switch (preference) {
#if TARGET_MACOS
        case display_configuration::METAL: {
            auto backend = new metal::driver(m_config.display);
            m_api.info = metal::driver::api_info();
            m_api.backend = reinterpret_cast<void *>(backend);
            m_api.bindings = backend->api_bindings();
            break;
        }
#endif
#if !TARGET_MACOS
        case display_configuration::VULKAN: {
            // Fallthrough to OPENGL/DIRECTX for now.
        }
#endif
#if TARGET_WINDOWS
        case display_configuration::DIRECTX: {
            // Fallthrough to OPENGL for now.
        }
#endif
        case display_configuration::OPENGL: {
            auto backend = new opengl::driver(m_config.display);
            m_api.info = opengl::driver::api_info();
            m_api.backend = reinterpret_cast<void *>(backend);
            m_api.bindings = backend->api_bindings();
            break;
        }
        default:
            throw std::runtime_error("Unsupported backend graphics driver chosen (GL_" + std::to_string(preference) + ")");
    }
}

auto renderer::driver::start_driver(frame_request_callback frame_request) -> void
{
    if (!m_api.backend) {
        throw std::runtime_error("Attempting to start graphics driver, without one loaded.");
    }

    // Hook up any controllers that will respond to notifications from the backend.
    m_render.frame.initialize();
    m_render.frame_request = std::move(frame_request);

    m_api.bindings.delegate.attach_event_receiver(m_api.event_receiver);

    // Spin up the backend.
    m_api.bindings.core.start([&] {
        KESTREL_PROFILE_SCOPE("FRAME TIME");

        // Received request for a new frame to be rendered.
        start_frame();
        m_render.frame_request(m_render.frame);
        end_frame();
    });
}

// MARK: - Frame Management

auto renderer::driver::start_frame() -> void
{
    m_render.frame.begin();
}

auto renderer::driver::end_frame() -> void
{
    // Instruct the current frame to finish. This will set the current frame
    // to a busy state, and trigger the callback once all work is finished,
    // and which point the frame will be ended.
    m_render.frame.finalize([&] (auto callback) {
        m_api.bindings.frame_generation.finish(std::move(callback));
    });
}

// MARK: - Texture Management

auto renderer::driver::create_texture(const data::block &data, math::vec2 size) -> texture::id
{
    auto id = m_store.texture.aquire_new_id();
    auto info = m_store.texture.create_texture(id);
    info.handle = m_api.bindings.texture.create(data, size);
    info.size = size;
    info.gpu_live = true;
    info.use_count = 1;
    return id;
}

auto renderer::driver::update_texture(texture::id id, const data::block &data, math::vec2 size) -> void
{
    // We need to get a lock for the texture, and remove it
    auto& info = m_store.texture.get_texture(id);

    info.data = data;
    info.size = size;

    // Communicate with the backend driver, to update the actual texture in GPU memory (if it is live)
    if (info.gpu_live) {
        m_api.bindings.texture.update(info.handle, info.data);
    }

    m_store.texture.drop_texture(id);
}

auto renderer::driver::lock_texture(texture::id id) -> void
{
    m_store.texture.get_texture(id);
}

auto renderer::driver::unlock_texture(texture::id id) -> void
{
    m_store.texture.drop_texture(id);
}

auto renderer::driver::upload_texture(texture::id id) -> void
{
    auto& info = m_store.texture.get_texture(id);
    if (!info.gpu_live) {
        if (info.use_count <= 1) {
            // TODO: Issue a warning here about unnecessary use of GPU Resources.
        }
        // Tell backend to create the texture
        info.handle = m_api.bindings.texture.create(info.data, info.size);
        if (info.handle) {
            info.gpu_live = true;
        }
    }
    m_store.texture.drop_texture(id);
}

auto renderer::driver::device_texture_id(texture::id id) const -> texture::device_id
{
    const auto& info = m_store.texture.get_texture_temporary(id);
    return info.handle;
}

// MARK: - Frame Operations

auto renderer::driver::draw(const renderer::buffer &buffer) const -> void
{
    m_api.bindings.frame_generation.submit_draw_buffer(buffer);
}

// MARK: - Events

auto renderer::driver::set_event_receiver(::event::receiver *receiver) -> void
{
    m_api.event_receiver = receiver;
    if (m_api.backend) {
        m_api.bindings.delegate.attach_event_receiver(receiver);
    }
}