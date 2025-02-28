#pragma once

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/window.h"

struct GLFWwindow;

namespace elm {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& spec);
		virtual ~WindowsWindow();

		void on_update(void) override;

		inline uint32_t get_width(void) const override { return m_data.width; }
		inline uint32_t get_height(void) const override { return m_data.width; }

		// Window attributes
		void set_event_callback(const EventCallbackFn& cb) override;
		void set_vsync(bool enabled) override;
		inline bool is_vsync(void) const override { return m_data.vsync; }

		void *get_native_window(void) override { return (void *)m_window; }

	private:
		virtual void init(const WindowSpecification& specs);
		virtual void shutdown(void);

	private:
		GLFWwindow* m_window;

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
