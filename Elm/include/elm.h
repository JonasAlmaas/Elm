#pragma once

// --- Core -----------------
#include <elm/core/base.h>

#include <elm/core/application.h>
#include <elm/core/file_dialog.h>
#include <elm/core/layer.h>
#include <elm/core/log.h>
#include <elm/core/time.h>
#include <elm/core/timestep.h>

#include <elm/core/event/event.h>
#include <elm/core/event/key_event.h>
#include <elm/core/event/mouse_event.h>
#include <elm/core/event/window_event.h>

#include <elm/core/input/input.h>
#include <elm/core/input/key_code.h>
#include <elm/core/input/mouse_code.h>
// --------------------------

// --- Core - Renderer ------
#include <elm/core/renderer/frame_buffer.h>
#include <elm/core/renderer/index_buffer.h>
#include <elm/core/renderer/render_command.h>
#include <elm/core/renderer/shader.h>
#include <elm/core/renderer/shader_library.h>
#include <elm/core/renderer/texture.h>
#include <elm/core/renderer/uniform_buffer.h>
#include <elm/core/renderer/vertex_array.h>
#include <elm/core/renderer/vertex_buffer.h>
#include <elm/core/renderer/vertex_buffer_element.h>
#include <elm/core/renderer/vertex_buffer_layout.h>
// --------------------------

// --- Math -----------------
#include <elm/math/math.h>
// --------------------------

// --- Renderer -------------
#include <elm/renderer/renderer.h>
#include <elm/renderer/renderer_2d.h>
#include <elm/renderer/scene_renderer.h>

#include <elm/renderer/mesh.h>

#include <elm/camera/orthographic_camera.h>
#include <elm/camera/orthographic_camera_controller.h>
#include <elm/camera/perspective_camera.h>
#include <elm/camera/perspective_camera_controller.h>
// --------------------------

// --- Scene ----------------
#include <elm/scene/scene.h>
#include <elm/scene/entity.h>
#include <elm/scene/components.h>
// --------------------------
