#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	struct editor_layer : layer
	{
		editor_layer(void);
		virtual ~editor_layer(void) = default;

		virtual void on_attach(void) override;
		virtual void on_detach(void) override;
		virtual void on_update(timestep ts) override;
		virtual void on_event(event &e) override;
		virtual void on_imgui_render(void) override;

	private:
		std::shared_ptr<frame_buffer> frame_buffer;

		glm::vec2 viewport_size = { 0.0f, 0.0f };
		bool viewport_focused = false;
		bool viewport_hovered = false;
	};
}
