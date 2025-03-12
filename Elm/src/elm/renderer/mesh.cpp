#include "mesh.h"

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include <vector>

namespace elm {

	struct mesh_vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
	};

	std::shared_ptr<mesh> mesh::create(const std::filesystem::path &fpath)
	{
		auto ret = std::make_shared<mesh>();

		std::vector<mesh_vertex> vertices;

		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warining, error;

		std::string fpath_str = fpath.string().c_str();
		const char *fpath_cstr = fpath_str.c_str();

		bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warining, &error, fpath_cstr);
		if (!warining.empty()) {
			ELM_CORE_WARN("Warning: \"{}\" loading model \"{}\"", fpath.string(), warining);
		}
		if (!error.empty()) {
			ELM_CORE_WARN("Error: \"{}\" loading model \"{}\"", fpath.string(), error);
		}

		if (!success) {
			ELM_CORE_WARN("Failed to load obj model \"{0}\"", fpath.string());
			ELM_CORE_ASSERT(false, "");
			return ret;
		}

		for (const auto &shape : shapes) {
			for (const auto &ix : shape.mesh.indices) {
				vertices.emplace_back(
					glm::vec3( // Position
						attributes.vertices[3 * ix.vertex_index],
						attributes.vertices[3 * ix.vertex_index + 1],
						attributes.vertices[3 * ix.vertex_index + 2]),
					glm::vec2( // UV
						attributes.texcoords[2 * ix.texcoord_index],
						attributes.texcoords[2 * ix.texcoord_index + 1]),
					glm::vec3( // Normal
						attributes.normals[3 * ix.normal_index],
						attributes.normals[3 * ix.normal_index + 1],
						attributes.normals[3 * ix.normal_index + 2])
				);
			}
		}

		auto va = vertex_array::create();

		auto vb = vertex_buffer::create((const void *)vertices.data(), (uint32_t)vertices.size() * sizeof(mesh_vertex));
		vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" },
			{ elm::shader_data_type::Float3, "a_normal" } };
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
