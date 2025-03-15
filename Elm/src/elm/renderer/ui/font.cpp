#include "font.h"

#undef INFINITE
#include <msdf-atlas-gen.h>
#include <FontGeometry.h>
#include <GlyphGeometry.h>

namespace elm {

	struct msdf_data {
		msdf_atlas::FontGeometry font_geometry;
		std::vector<msdf_atlas::GlyphGeometry> glyphs;
	};

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static std::shared_ptr<texture_2d> create_n_cache_atlas(
		const std::string &font_name,
		float font_size,
		const std::vector<msdf_atlas::GlyphGeometry> &glyphs,
		const msdf_atlas::FontGeometry &font_geometry,
		uint32_t width,
		uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		texture_2d_specification spec = {
			/*spec.format = image_format::RGB8;
			spec.generate_mips = false;*/
		};
		auto texture = texture_2d::create(bitmap.width, bitmap.height, spec);
		texture->set_data((void *)bitmap.pixels, bitmap.width * bitmap.height * 3);

		return texture;
	}

	font::font(const std::filesystem::path &fpath)
		: m_data(new msdf_data())
	{
		auto ft = msdfgen::initializeFreetype();
		ELM_CORE_ASSERT(ft);

		std::string fpath_str = fpath.string();

		auto font = loadFont(ft, fpath_str.c_str());
		if (!font) {
			ELM_CORE_ERROR("Failed to load font {}", fpath_str);
			return;
		}

		// From imgui_draw.cpp
		struct charset_range {
			uint32_t begin, end;
		};
		static const charset_range charset_ranges[] =
		{
			{ 0x0020, 0x00FF }
		};

		msdf_atlas::Charset charset;
		for (auto range : charset_ranges) {
			for (uint32_t c = range.begin; c <= range.end; ++c) {
				charset.add(c);
			}
		}

		double font_scale = 1.0;
		m_data->font_geometry = msdf_atlas::FontGeometry(&m_data->glyphs);
		int glyphs_loaded = m_data->font_geometry.loadCharset(font, font_scale, charset);
		ELM_CORE_INFO("Loaded {} glyphs from font (out of {})", glyphs_loaded, charset.size());



		double em_size = 40.0;

		msdf_atlas::TightAtlasPacker atlas_packer;
		// atlas_packer.setDimensionsConstraint()
		atlas_packer.setPixelRange(2.0);
		atlas_packer.setMiterLimit(1.0);
		atlas_packer.setInnerUnitPadding(0);
		atlas_packer.setOuterUnitPadding(0);
		atlas_packer.setScale(em_size);
		int remaining = atlas_packer.pack(m_data->glyphs.data(), (int)m_data->glyphs.size());
		ELM_CORE_ASSERT(remaining == 0);

		int width, height;
		atlas_packer.getDimensions(width, height);
		em_size = atlas_packer.getScale();

		m_atlas_texture = create_n_cache_atlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", (float)em_size, m_data->glyphs, m_data->font_geometry, width, height);

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}

	font::~font(void)
	{
		delete m_data;
	}
}
