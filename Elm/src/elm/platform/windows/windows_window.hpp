#pragma once

#include "elm/core/base.hpp"

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/window.hpp"
#include "elm/core/renderer/graphics_context.hpp"

struct GLFWwindow;

#include <memory>

namespace elm {

	struct windows_window : window
	{
		windows_window(const window_specification& spec);
		virtual ~windows_window();

		virtual void on_update(bool minimized) override;

		virtual inline uint32_t get_width(void) const override { return this->data.width; }
		virtual inline uint32_t get_height(void) const override { return this->data.height; }

		// Window attributes
		virtual void set_event_callback(const std::function<void(event &)> & cb) override;
		virtual void set_vsync(bool enabled) override;
		virtual inline bool is_vsync(void) const override { return this->data.vsync; }

		virtual void enable_cursor(void) override;
		virtual void disable_cursor(void) override;

		virtual void *get_native_window(void) override { return (void *)this->window; }

	private:
		virtual void init(const window_specification& specs);
		virtual void shutdown(void);

	private:
		GLFWwindow* window;
		std::unique_ptr<graphics_context> context;

		struct WindowData {
			std::string title;
			uint32_t width, height;
			bool vsync;

			std::function<void(event &)> event_callback;
		};

		WindowData data;
	};
}

#endif /* ELM_PLATFORM_WINDOWS */
