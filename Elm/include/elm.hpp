#pragma once

// --- Core -----------------
#include <elm/core/base.hpp>

#include <elm/core/application.hpp>
#include <elm/core/file_dialog.hpp>
#include <elm/core/layer.hpp>
#include <elm/core/log.hpp>
#include <elm/core/time.hpp>
#include <elm/core/timestep.hpp>

#include <elm/core/event/event.hpp>
#include <elm/core/event/key_event.hpp>
#include <elm/core/event/mouse_event.hpp>
#include <elm/core/event/window_event.hpp>

#include <elm/core/input/input.hpp>
#include <elm/core/input/key_code.hpp>
#include <elm/core/input/mouse_code.hpp>
// --------------------------

// --- Core - Renderer ------
#include <elm/core/renderer/frame_buffer.hpp>
#include <elm/core/renderer/index_buffer.hpp>
#include <elm/core/renderer/render_command.hpp>
#include <elm/core/renderer/shader.hpp>
#include <elm/core/renderer/shader_library.hpp>
#include <elm/core/renderer/texture.hpp>
#include <elm/core/renderer/uniform_buffer.hpp>
#include <elm/core/renderer/vertex_array.hpp>
#include <elm/core/renderer/vertex_buffer.hpp>
#include <elm/core/renderer/vertex_buffer_element.hpp>
#include <elm/core/renderer/vertex_buffer_layout.hpp>
// --------------------------

// --- Math -----------------
#include <elm/math/math.hpp>
#include <elm/math/spline.hpp>
// --------------------------

// --- Renderer -------------
#include <elm/renderer/renderer.hpp>
#include <elm/renderer/renderer_2d.hpp>
#include <elm/renderer/scene_renderer.hpp>

#include <elm/renderer/cubemap.hpp>
#include <elm/renderer/font.hpp>
#include <elm/renderer/material.hpp>
#include <elm/renderer/mesh.hpp>

#include <elm/camera/orthographic_camera.hpp>
#include <elm/camera/orthographic_camera_controller.hpp>
#include <elm/camera/perspective_camera.hpp>
#include <elm/camera/perspective_camera_controller.hpp>
// --------------------------

// --- Scene ----------------
#include <elm/scene/scene.hpp>
#include <elm/scene/entity.hpp>
#include <elm/scene/components.hpp>
// --------------------------

// --- Misc -----------------
#include <elm/random.hpp>
// --------------------------
