#pragma once

#undef INFINITE
#include <msdf-atlas-gen.h>
#include <vector>

namespace elm {

	struct msdf_data
	{
		msdf_atlas::FontGeometry font_geometry;
		std::vector<msdf_atlas::GlyphGeometry> glyphs;
	};
}
