#include "elm/core/file_dialog.hpp"

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/application.hpp"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace elm::file_dialog {

	extern std::optional<std::string> open_file(const char *filter)
	{
		ELM_PROFILE_FUNCTION();

		OPENFILENAMEA ofn;

		CHAR sz_file[260] = { 0 };
		CHAR current_dir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)application::get()->get_window()->get_native_window());
		ofn.lpstrFile = sz_file;
		ofn.nMaxFile = sizeof sz_file;
		if (GetCurrentDirectoryA(256, current_dir)) {
			ofn.lpstrInitialDir = current_dir;
		}
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		} else {
			return std::nullopt;
		}
	}

	extern std::optional<std::string> save_file(const char *filter)
	{
		ELM_PROFILE_FUNCTION();

		OPENFILENAMEA ofn;

		CHAR sz_file[260] = { 0 };
		CHAR current_dir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)application::get()->get_window()->get_native_window());
		ofn.lpstrFile = sz_file;
		ofn.nMaxFile = sizeof sz_file;
		if (GetCurrentDirectoryA(256, current_dir)) {
			ofn.lpstrInitialDir = current_dir;
		}
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE) {
			return ofn.lpstrFile;
		} else {
			return std::nullopt;
		}
	}
}

#endif // ELM_PLATFORM_WINDOWS
