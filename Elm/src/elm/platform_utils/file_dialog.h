#pragma once

#include "elm/core/base.h"
#include <string>

namespace elm::file_dialog {

	/**
	 * @return path to file. Empty string if canceled.
	 */
	extern std::string open_file(const char *filter);

	/**
	 * @return path to directory. Empty string if canceled.
	 */
	extern std::string save_file(const char *filter);
}
