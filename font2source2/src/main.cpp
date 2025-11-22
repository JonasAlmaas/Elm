#include <msdf-atlas-gen.h>
#include <FontGeometry.h>
#include <GlyphGeometry.h>

#include <stb_image_write.h>

#include <glm/glm.hpp>

#include <iostream>
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
	uint64_t glyph_seed = coloring_seed;
	for (auto &glyph : msdf_data->glyphs) {
		glyph_seed *= LCG_MULTIPLIER;
		//glyph.edgeColoring(msdfgen::edgeColoringSimple, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
		glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
		//glyph.edgeColoring(msdfgen::edgeColoringByDistance, DEFAULT_ANGLE_THRESHOLD, glyph_seed);
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
	std::map<std::pair<uint32_t, uint32_t>, double> kerning;
};

static void create_glyph_model(
	const msdf_atlas::GlyphGeometry *glyph,
	const std::string &font_name,
	uint32_t atlas_width,
	uint32_t atlas_height,
	const std::filesystem::path &fpath_vmdl,
	const std::filesystem::path &fpath_smd)
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
	uv_min *= glm::vec2(texel_width, texel_height);
	uv_max *= glm::vec2(texel_width, texel_height);

	std::ofstream ofs_smd(fpath_smd);
	ofs_smd << "version 1\n"
		<< "nodes\n"
		<< "0 \"root\" -1\n"
		<< "end\n"
		<< "skeleton\n"
		<< "time 0\n"
		<< "0 0 0 0 0 0 0\n"
		<< "end\n"
		<< "triangles\n";

	ofs_smd << "no_material\n";
	ofs_smd << "0  " << quad_min.x << " " << quad_min.y << " 0.000000  "
		<< "0.000000 0.000000 1.000000  "
		<< uv_min.x << " " << uv_min.y << "\n";
	ofs_smd << "0  " << quad_max.x << " " << quad_max.y << " 0.000000  "
		<< "0.000000 0.000000 1.000000  "
		<< uv_max.x << " " << uv_max.y << "\n";
	ofs_smd << "0  " << quad_min.x << " " << quad_max.y << " 0.000000  "
		<< "0.000000 0.000000 1.000000  "
		<< uv_min.x << " " << uv_max.y << "\n";

	ofs_smd << "no_material\n";
	ofs_smd << "0  " << quad_min.x << " " << quad_min.y << " 0.000000  "
		<< "0.000000 0.000000 1.000000  "
		<< uv_min.x << " " << uv_min.y << "\n";
	ofs_smd << "0  " << quad_max.x << " " << quad_min.y << " 0.000000  "
		<< "0.000000 0.000000 1.000000  "
		<< uv_max.x << " " << uv_min.y << "\n";
	ofs_smd << "0  " << quad_max.x << " " << quad_max.y << " 0.000000  "
		<< "0.000000 0.000000 1.000000  "
		<< uv_max.x << " " << uv_max.y << "\n";

	ofs_smd << "end\n";

	std::ofstream ofs_vmdl(fpath_vmdl);

	ofs_vmdl << R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d} format:modeldoc40:version{da0ab1f8-9722-4910-94b8-10b6a08c0934} -->
{
	rootNode =
	{
		_class = "RootNode"
		children = 
		[
			{
				_class = "MaterialGroupList"
				children = 
				[
					{
						_class = "DefaultMaterialGroup"
						use_global_default = true
						global_default_material = "materials/text/)";
	ofs_vmdl << font_name;
	ofs_vmdl << R"(.vmat"
					},
				]
			},
			{
				_class = "RenderMeshList"
				children = 
				[
					{
						_class = "RenderMeshFile"
						filename = "models/text/)";

	ofs_vmdl << fpath_smd.filename().string();
	ofs_vmdl << R"("
						import_scale = 1.0
						import_filter = 
						{
							exclude_by_default = false
							exception_list = [  ]
						}
					},
				]
			},
		]
		model_archetype = ""
		primary_associated_entity = ""
		anim_graph_name = ""
		document_sub_type = "ModelDocSubType_None"
	}
})";
}

int main(void)
{
	const char *font_path_str = "./OpenSans-Medium.ttf";
	std::filesystem::path font_path(font_path_str);

	std::string font_name = font_path.stem().string();

	std::filesystem::path out_meta_fpath = "./meta.json";

	std::filesystem::path out_materials_dir = ".\\addon\\materials\\text";
	std::filesystem::create_directories(out_materials_dir);
	std::filesystem::path out_models_dir = ".\\addon\\models\\text";
	std::filesystem::create_directories(out_models_dir);

	std::filesystem::path out_atlas_path = out_materials_dir / (font_name + ".png");

	std::vector<charset_range> charset_ranges = { { 0x0020, 0x007E} };

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
			std::stringstream ss;
			ss << std::setfill('0') << std::setw(4) << std::hex << c;

			auto glyph = msdf_data.font_geometry.getGlyph(c);
			if (!glyph) {
				std::cerr << "Failed to load glyph for " << ss.str() << "\n";
				continue;
			}

			create_glyph_model(
				glyph,
				font_name,
				msdf_data.atlas_width,
				msdf_data.atlas_height,
				out_models_dir / (font_name + "_" + ss.str() + ".vmdl"),
				out_models_dir / (font_name + "_" + ss.str() + ".smd"));

			result.glyph_meta[c] = {
				.advance = glyph->getAdvance(),
			};
		}

		// Load kerning
		for (size_t i = range.begin; i < range.end; ++i) {
			for (size_t j = range.begin; j < range.end; ++j) {
				double advance;
				msdf_data.font_geometry.getAdvance(advance, i, j);
				result.kerning[std::make_pair<uint32_t, uint32_t>(i, j)] = /*geometryScale **/ advance;
			}
		}
	}

	std::ofstream ofs_meta(out_meta_fpath);

	for (auto &gmeta : result.glyph_meta) {
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(4) << std::hex << gmeta.first;

		const auto &meta = gmeta.second;

		ofs_meta << '"' << ss.str() << "\":{\"advance\":" << meta.advance << "},\n";
	}

	for (auto [key, advance] : result.kerning) {
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(4) << std::hex << key.first
			<< "->" << std::setfill('0') << std::setw(4) << std::hex << key.second;

		ofs_meta << '"' << ss.str() << "\":" << advance << ",\n";
	}

	return EXIT_SUCCESS;
}
