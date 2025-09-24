#include <msdf-atlas-gen.h>
#include <FontGeometry.h>
#include <GlyphGeometry.h>

#include <stb_image_write.h>

#include <stdlib.h>
#include <memory>

template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
static void create_n_store_atlas(
	const char *out_atlas_path,
	const std::vector<msdf_atlas::GlyphGeometry> &glyphs,
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

	const int channels = 3;

	stbi_flip_vertically_on_write(1);
	stbi_write_png(out_atlas_path, bitmap.width, bitmap.height, channels, bitmap.pixels, bitmap.width * channels);

#if 0
	texture_specification spec = {
		.width = (uint32_t)bitmap.width,
		.height = (uint32_t)bitmap.height,
		.format = image_format::RGB8,
		/*spec.generate_mips = false, */
	};
	auto texture = texture_2d::create(spec);
	texture->set_data((void *)bitmap.pixels, bitmap.width * bitmap.height * 3);

	return texture;
#endif
}

struct msdf_data_s {
	msdf_atlas::FontGeometry font_geometry;
	std::vector<msdf_atlas::GlyphGeometry> glyphs;
};

static void create_font(
	const char *font_path,
	msdf_data_s *msdf_data,
	const char *out_atlas_path)
{
	auto ft = msdfgen::initializeFreetype();
	if (!ft) exit(1);

	auto font = msdfgen::loadFont(ft, font_path);
	if (!font) exit(1);

	// From imgui_draw.cpp
	struct charset_range { uint32_t begin, end; };
	static const charset_range charset_ranges[] = { { 0x0020, 0x00FF } };

	msdf_atlas::Charset charset;
	for (auto range : charset_ranges) {
		for (uint32_t c = range.begin; c <= range.end; ++c) {
			charset.add(c);
		}
	}

	std::vector<msdf_atlas::GlyphGeometry> glyphs;
	msdf_atlas::FontGeometry font_geometry;

	double font_scale = 1.0;
	font_geometry = msdf_atlas::FontGeometry(&glyphs);
	int glyphs_loaded = font_geometry.loadCharset(font, font_scale, charset);

	double em_size = 40.0;

	msdf_atlas::TightAtlasPacker atlas_packer;
	// atlas_packer.setDimensionsConstraint()
	atlas_packer.setPixelRange(2.0);
	atlas_packer.setMiterLimit(1.0);
	atlas_packer.setInnerUnitPadding(0);
	atlas_packer.setOuterUnitPadding(0);
	atlas_packer.setScale(em_size);
	int remaining = atlas_packer.pack(glyphs.data(), (int)glyphs.size());
	if (remaining != 0) exit(1);

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
		msdf_atlas::Workload([&glyphs = glyphs, &coloring_seed](int i, int thread_no) -> bool {
			uint64_t glyph_seed = (LCG_MULTIPLIER * (coloring_seed ^ i) + LCG_INCREMENT) * !!coloring_seed;
			glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
			return true;
			}, (int)glyphs.size()).finish(THREAD_COUNT);
	} else {
		uint64_t glyph_seed = coloring_seed;
		for (auto &glyph : glyphs) {
			glyph_seed *= LCG_MULTIPLIER;
			glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
		}
	}

	create_n_store_atlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>(
		out_atlas_path,
		glyphs,
		width,
		height);

	msdfgen::destroyFont(font);
	msdfgen::deinitializeFreetype(ft);
}

int main(void)
{
	const char *font_path = "./OpenSans-Medium.ttf";
	const char *out_atlas_path = "./atlas.png";

	msdf_data_s msdf_data;
	create_font(font_path, &msdf_data, out_atlas_path);

	return EXIT_SUCCESS;
}
