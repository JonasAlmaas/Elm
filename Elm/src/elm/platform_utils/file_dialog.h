#pragma once

#include "elm/core/base.h"
#include <string>

namespace elm::file_dialog {

	/**
	 * @param filter E.g. "Model file (*.fbx)\0*.fbx\0"
	 * @return path to file. Empty string if canceled.
	 */
	extern std::string open_file(const char *filter);

	/**
	 * @param filter E.g. "Model file (*.fbx)\0*.fbx\0"
	 * @return path to directory. Empty string if canceled.
	 */
	extern std::string save_file(const char *filter);
}
