#pragma once

#include "elm/core/base.h"

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/window.h"
#include "elm/core/renderer/graphics_context.h"

struct GLFWwindow;

#include <memory>

namespace elm {

	struct windows_window : window
	{
		windows_window(const window_specification& spec);
		virtual ~windows_window();

		virtual void on_update(bool minimized) override;

		virtual inline uint32_t get_width(void) const override { return m_data.width; }
		virtual inline uint32_t get_height(void) const override { return m_data.width; }

		// Window attributes
		virtual void set_event_callback(const std::function<void(event &)> & cb) override;
		virtual void set_vsync(bool enabled) override;
		virtual inline bool is_vsync(void) const override { return m_data.vsync; }

		virtual void enable_cursor(void) override;
		virtual void disable_cursor(void) override;

		virtual void *get_native_window(void) override { return (void *)m_window; }

	private:
		virtual void init(const window_specification& specs);
		virtual void shutdown(void);

	private:
		GLFWwindow* m_window;
		std::unique_ptr<graphics_context> m_context;

		struct WindowData {
			std::string title;
			uint32_t width, height;
			bool vsync;

			std::function<void(event &)> event_callback;
		};

		WindowData m_data;
	};
}

#endif /* ELM_PLATFORM_WINDOWS */
