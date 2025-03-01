#pragma once

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/window.h"
#include "elm/core/renderer/graphics_context.h"

struct GLFWwindow;

#include <memory>

namespace elm {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& spec);
		virtual ~WindowsWindow();

		virtual void on_update(void) override;

		virtual inline uint32_t get_width(void) const override { return m_data.width; }
		virtual inline uint32_t get_height(void) const override { return m_data.width; }

		// Window attributes
		virtual void set_event_callback(const EventCallbackFn& cb) override;
		virtual void set_vsync(bool enabled) override;
		virtual inline bool is_vsync(void) const override { return m_data.vsync; }

		virtual void *get_native_window(void) override { return (void *)m_window; }

	private:
		virtual void init(const WindowSpecification& specs);
		virtual void shutdown(void);

	private:
		GLFWwindow* m_window;
		std::unique_ptr<GraphicsContext> m_context;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			bool vsync;

			EventCallbackFn event_callback;
		};

		WindowData m_data;
	};
}

#endif /* ELM_PLATFORM_WINDOWS */
