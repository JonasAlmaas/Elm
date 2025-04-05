#include "font.hpp"

#include "msdf_data.hpp"
#include <FontGeometry.h>
#include <GlyphGeometry.h>

namespace elm {

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static std::shared_ptr<texture_2d> create_atlas(
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

		texture_specification spec = {
			.width = (uint32_t)bitmap.width,
			.height = (uint32_t)bitmap.height,
			.format = image_format::RGB8,
			/*spec.generate_mips = false, */
		};
		auto texture = texture_2d::create(spec);
		texture->set_data((void *)bitmap.pixels, bitmap.width * bitmap.height * 3);

		return texture;
	}

	font::font(const std::filesystem::path &fpath)
		: data(new msdf_data())
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
		this->data->font_geometry = msdf_atlas::FontGeometry(&this->data->glyphs);
		int glyphs_loaded = this->data->font_geometry.loadCharset(font, font_scale, charset);
		ELM_CORE_INFO("Loaded {} glyphs from font (out of {})", glyphs_loaded, charset.size());

		double em_size = 40.0;

		msdf_atlas::TightAtlasPacker atlas_packer;
		// atlas_packer.setDimensionsConstraint()
		atlas_packer.setPixelRange(2.0);
		atlas_packer.setMiterLimit(1.0);
		atlas_packer.setInnerUnitPadding(0);
		atlas_packer.setOuterUnitPadding(0);
		atlas_packer.setScale(em_size);
		int remaining = atlas_packer.pack(this->data->glyphs.data(), (int)this->data->glyphs.size());
		ELM_CORE_ASSERT(remaining == 0);

		int width, height;
		atlas_packer.getDimensions(width, height);
		em_size = atlas_packer.getScale();

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define THREAD_COUNT 8
		// if MSDF || MTSDF

		uint64_t coloring_seed = 0;
		bool expensive_coloring = false;
		if (expensive_coloring) {
			msdf_atlas::Workload([&glyphs = this->data->glyphs, &coloring_seed](int i, int thread_no) -> bool {
				uint64_t glyph_seed = (LCG_MULTIPLIER * (coloring_seed ^ i) + LCG_INCREMENT) * !!coloring_seed;
				glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
				return true;
			}, (int)this->data->glyphs.size()).finish(THREAD_COUNT);
		} else {
			uint64_t glyph_seed = coloring_seed;
			for (auto &glyph : this->data->glyphs) {
				glyph_seed *= LCG_MULTIPLIER;
				glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
			}
		}

		this->atlas_texture = create_atlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>(
			"Test",
			(float)em_size,
			this->data->glyphs,
			this->data->font_geometry,
			width,
			height);

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
	}

	font::~font(void)
	{
		delete this->data;
	}

	std::shared_ptr<font> font::create(const std::filesystem::path &fpath)
	{
		return std::make_shared<font>(fpath);
	}

	std::shared_ptr<font> font::get_default(void)
	{
		static std::shared_ptr<font> s_default;
		if (!s_default) {
			s_default = std::make_shared<font>("content/fonts/opensans/OpenSans-Regular.ttf");
		}
		return s_default;
	}
}
