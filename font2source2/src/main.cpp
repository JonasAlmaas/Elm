#include <msdf-atlas-gen.h>
#include <FontGeometry.h>
#include <GlyphGeometry.h>

#include <stb_image_write.h>

#include <glm/glm.hpp>

#include <stdlib.h>
#include <memory>
#include <filesystem>
#include <fstream>

struct charset_range {
	uint32_t begin, end;
};

struct msdf_data_s {
	msdf_atlas::FontGeometry font_geometry;
	std::vector<msdf_atlas::GlyphGeometry> glyphs;
	uint32_t atlas_width, atlas_height;
};

template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
static void create_n_store_atlas(
	const std::filesystem::path &out_atlas_path,
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
	stbi_write_png(out_atlas_path.string().c_str(), bitmap.width, bitmap.height, channels, bitmap.pixels, bitmap.width * channels);
}

static void create_font(
	const char *font_path,
	msdf_data_s *msdf_data,
	const std::filesystem::path &out_atlas_path,
	const std::vector<charset_range> &charset_ranges)
{
	auto ft = msdfgen::initializeFreetype();
	if (!ft) exit(1);

	auto font = msdfgen::loadFont(ft, font_path);
	if (!font) exit(1);

	msdf_atlas::Charset charset;
	for (auto &range : charset_ranges) {
		for (uint32_t c = range.begin; c <= range.end; ++c) {
			charset.add(c);
		}
	}

	//std::vector<msdf_atlas::GlyphGeometry> glyphs;
	//msdf_atlas::FontGeometry font_geometry;

	double font_scale = 1.0;
	msdf_data->font_geometry = msdf_atlas::FontGeometry(&msdf_data->glyphs);
	int glyphs_loaded = msdf_data->font_geometry.loadCharset(font, font_scale, charset);

	double em_size = 40.0;

	msdf_atlas::TightAtlasPacker atlas_packer;
	// atlas_packer.setDimensionsConstraint()
	atlas_packer.setPixelRange(2.0);
	atlas_packer.setMiterLimit(1.0);
	atlas_packer.setInnerUnitPadding(0);
	atlas_packer.setOuterUnitPadding(0);
	atlas_packer.setScale(em_size);
	int remaining = atlas_packer.pack(msdf_data->glyphs.data(), (int)msdf_data->glyphs.size());
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
		msdf_atlas::Workload([&glyphs = msdf_data->glyphs, &coloring_seed](int i, int thread_no) -> bool {
			uint64_t glyph_seed = (LCG_MULTIPLIER * (coloring_seed ^ i) + LCG_INCREMENT) * !!coloring_seed;
			glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
			return true;
			}, (int)msdf_data->glyphs.size()).finish(THREAD_COUNT);
	} else {
		uint64_t glyph_seed = coloring_seed;
		for (auto &glyph : msdf_data->glyphs) {
			glyph_seed *= LCG_MULTIPLIER;
			glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
		}
	}

	create_n_store_atlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>(
		out_atlas_path,
		msdf_data->glyphs,
		width,
		height);

	msdf_data->atlas_width = width;
	msdf_data->atlas_height = height;

	msdfgen::destroyFont(font);
	msdfgen::deinitializeFreetype(ft);
}

struct font_metrics_s {
	/// The size of one EM.
	double em_size;
	/// The vertical position of the ascender and descender relative to the baseline.
	double ascender_y, descender_y;
	/// The vertical difference between consecutive baselines.
	double line_height;
	/// The vertical position and thickness of the underline.
	double underline_y, underline_thickness;
};

struct glyph_meta_s {
	double advance;
};

struct font_meta_s {
	font_metrics_s font_metrics;

	std::unordered_map<uint32_t, glyph_meta_s> glyph_meta;
};

static void create_glyph_model(
	const msdf_atlas::GlyphGeometry *glyph,
	uint32_t atlas_width,
	uint32_t atlas_height,
	const std::filesystem::path &fpath)
{
	double al, ab, ar, at;
	glyph->getQuadAtlasBounds(al, ab, ar, at);

	glm::vec2 uv_min((float)al, (float)ab);
	glm::vec2 uv_max((float)ar, (float)at);

	double pl, pb, pr, pt;
	glyph->getQuadPlaneBounds(pl, pb, pr, pt);
	glm::vec2 quad_min((float)pl, (float)pb);
	glm::vec2 quad_max((float)pr, (float)pt);

	float texel_width = 1.0f / atlas_width;
	float texel_height = 1.0f / atlas_height;

	std::ofstream ofs(fpath);
	ofs << "# font2source2\n";
	ofs << "o Plane\n";

	ofs << "v " << quad_min.x << " 0.000000 " << quad_max.y << "\n";
	ofs << "v " << quad_max.x << " 0.000000 " << quad_max.y << "\n";
	ofs << "v " << quad_min.x << " 0.000000 " << quad_min.y << "\n";
	ofs << "v " << quad_max.x << " 0.000000 " << quad_min.y << "\n";

	ofs << "vt " << uv_min.x << " 0.000000 " << uv_max.y << "\n";
	ofs << "vt " << uv_max.x << " 0.000000 " << uv_max.y << "\n";
	ofs << "vt " << uv_min.x << " 0.000000 " << uv_min.y << "\n";
	ofs << "vt " << uv_max.x << " 0.000000 " << uv_min.y << "\n";

	ofs << "s 0\n";
	ofs << "f 1/1 2/2 4/3 3/4\n";
}

int main(void)
{
	const char *font_path_str = "./OpenSans-Medium.ttf";
	std::filesystem::path font_path(font_path_str);

	std::string font_name = font_path.stem().string();

	std::filesystem::path out_materials_dir = ".\\addon\\materials\\text";
	std::filesystem::create_directories(out_materials_dir);
	std::filesystem::path out_models_dir = ".\\addon\\models\\text";
	std::filesystem::create_directories(out_models_dir);

	std::filesystem::path out_atlas_path = out_materials_dir / (font_name + ".png");

	std::vector<charset_range> charset_ranges = { { 0x0020, 0x00FF } };

	msdf_data_s msdf_data;
	create_font(font_path_str, &msdf_data, out_atlas_path, charset_ranges);

	font_meta_s result;

	const auto &metrics = msdf_data.font_geometry.getMetrics();
	result.font_metrics = {
		.em_size=metrics.emSize,
		.ascender_y=metrics.ascenderY,
		.descender_y=metrics.descenderY,
		.line_height=metrics.lineHeight,
		.underline_y=metrics.underlineY,
		.underline_thickness=metrics.underlineThickness};

	for (auto &range : charset_ranges) {
		for (uint32_t c = range.begin; c <= range.end; ++c) {
			auto glyph = msdf_data.font_geometry.getGlyph(c);
			if (!glyph) exit(1);

			std::stringstream ss;
			ss << std::setfill('0') << std::setw(4) << std::hex << c;

			create_glyph_model(
				glyph,
				msdf_data.atlas_width,
				msdf_data.atlas_height,
				out_models_dir / (font_name + "_" + ss.str() + ".obj"));

			result.glyph_meta[c] = {
				.advance = glyph->getAdvance(),
			};
		}
	}

	return EXIT_SUCCESS;
}
