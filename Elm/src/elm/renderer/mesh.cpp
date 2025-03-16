#include "mesh.h"

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include <vector>

namespace elm {

	struct mesh_vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		int texture_slot;
	};

	std::shared_ptr<mesh> mesh::create(const std::filesystem::path &fpath)
	{
		auto ret = std::make_shared<mesh>();

		std::vector<mesh_vertex> vertices;

		tinyobj::ObjReaderConfig reader_config;
		reader_config.mtl_search_path = fpath.parent_path().string();

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(fpath.string(), reader_config)) {
			if (!reader.Error().empty()) {
				ELM_CORE_WARN("Error: \"{}\" loading model \"{}\"", fpath.string(), reader.Error());
			}

			ELM_CORE_WARN("Failed to load obj model \"{}\"", fpath.string());
			ELM_CORE_ASSERT(false, "");
			return ret;
		}

		if (!reader.Warning().empty()) {
			ELM_CORE_WARN("Warning: \"{}\" loading model \"{}\"", fpath.string(), reader.Warning());
		}

		auto &attrib = reader.GetAttrib();
		auto &shapes = reader.GetShapes();
		auto &materials = reader.GetMaterials();

		for (size_t s = 0; s < shapes.size(); ++s) {
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = (size_t)shapes[s].mesh.num_face_vertices[f];

				for (size_t v = 0; v < fv; v++) {
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					// Check if `normal_index` is zero or positive. negative = no normal data
					if (idx.normal_index < 0) {
						ELM_CORE_ASSERT(false, "We don't support models without normals");
						return ret;
					}

					// Check if `texcoord_index` is zero or positive. negative = no texcoord data
					if (idx.texcoord_index < 0) {
						ELM_CORE_ASSERT(false, "We don't support models without uvs");
						return ret;
					}

					// Optional: vertex colors
					// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
					// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
					// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

					vertices.emplace_back(
						glm::vec3( // Position
							attrib.vertices[3 * (size_t)idx.vertex_index + 0],
							attrib.vertices[3 * (size_t)idx.vertex_index + 1],
							attrib.vertices[3 * (size_t)idx.vertex_index + 2]),
						glm::vec2( // UV
							attrib.texcoords[2 * (size_t)idx.texcoord_index + 0],
							attrib.texcoords[2 * (size_t)idx.texcoord_index + 1]),
						glm::vec3( // Normal
							attrib.normals[3 * (size_t)idx.normal_index + 0],
							attrib.normals[3 * (size_t)idx.normal_index + 1],
							attrib.normals[3 * (size_t)idx.normal_index + 2]),
						// Texture slot
						shapes[s].mesh.material_ids[f] != -1 ? shapes[s].mesh.material_ids[f] : 0);
				}

				index_offset += fv;
			}
		}

		auto va = vertex_array::create();

		auto vb = vertex_buffer::create((const void *)vertices.data(), (uint32_t)vertices.size() * sizeof(mesh_vertex));
		vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" },
			{ elm::shader_data_type::Float3, "a_normal" },
			{ elm::shader_data_type::Int, "a_texture_slot" } };
		vb->set_layout(&layout);
		va->add_vertex_buffer(vb);

		std::vector<uint32_t> indices(vertices.size()); // Maybe there is a way to reuse triangles?
		for (uint32_t ix = 0; ix < vertices.size(); ++ix) {
			indices[ix] = ix;
		}
		auto ib = index_buffer::create(indices.data(), (uint32_t)indices.size());
		va->set_index_buffer(ib);

		ret->vertex_array = va;

		return ret;
	}
}
