#include "cubemap.h"
#include "elm/core/renderer/frame_buffer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/uniform_buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

// Temp
#include <glad/glad.h>

namespace elm::cubemap {

	static glm::mat4 s_capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	static glm::mat4 s_capture_views[] = {
		glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	static void render_cube(void)
	{
		static float s_vertices[] = {
			// Back
			-1.0f, -1.0f, -1.0f, // bottom-left
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f,  1.0f, -1.0f, // top-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			// Front
			-1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f,  1.0f,  1.0f, // top-right
			 1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, // top-left
			 1.0f,  1.0f,  1.0f, // top-right
			-1.0f, -1.0f,  1.0f, // bottom-left
			// Left
			-1.0f,  1.0f,  1.0f, // top-right
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f,  1.0f,  1.0f, // top-right
			// Right
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f, -1.0f,  1.0f, // bottom-left
			 // Bottom
			 -1.0f, -1.0f, -1.0f, // top-right
			  1.0f, -1.0f,  1.0f, // bottom-left
			  1.0f, -1.0f, -1.0f, // top-left
			 -1.0f, -1.0f,  1.0f, // bottom-right
			  1.0f, -1.0f,  1.0f, // bottom-left
			 -1.0f, -1.0f, -1.0f, // top-right
			 // Top
			 -1.0f,  1.0f, -1.0f, // top-left
			  1.0f,  1.0f, -1.0f, // top-right
			  1.0f,  1.0f , 1.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f, // top-left
			  1.0f,  1.0f,  1.0f, // bottom-right
			 -1.0f,  1.0f,  1.0f, // bottom-left
		};

		static std::shared_ptr<vertex_array> s_va;

		if (!s_va) {
			s_va = vertex_array::create();
			auto vb = vertex_buffer::create((const void *)s_vertices, sizeof s_vertices);
			vertex_buffer_layout layout = { { shader_data_type::FLOAT3, "a_position" } };
			vb->set_layout(&layout);
			s_va->add_vertex_buffer(vb);
		}

		s_va->bind();
		render_command::draw_triangles(36);
	}

	static void render_quad(void)
	{
		static float s_vertices[] = {
			// Positions        // UVs
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		static std::shared_ptr<vertex_array> s_va;

		if (!s_va) {
			s_va = vertex_array::create();
			auto vb = vertex_buffer::create((const void*)s_vertices, sizeof s_vertices);
			vertex_buffer_layout layout = {
				{ shader_data_type::FLOAT3, "a_position" },
				{ shader_data_type::FLOAT2, "a_uv" } };
			vb->set_layout(&layout);
			s_va->add_vertex_buffer(vb);
		}

		s_va->bind();
		render_command::draw_triangles(4);
	}

	extern std::shared_ptr<texture_cube> create(const std::filesystem::path &fpath, uint32_t size)
	{
		static std::shared_ptr<shader> s_equirectangular_to_cubemap_shader;
		if (!s_equirectangular_to_cubemap_shader) {
			s_equirectangular_to_cubemap_shader = shader::create("content/shaders/cubemap/equirectangular_to_cubemap.glsl");
		}

		uint32_t vp_x, vp_y, vp_width, vp_heigth;
		render_command::get_viewport(&vp_x, &vp_y, &vp_width, &vp_heigth);

		auto camera_ub = uniform_buffer::create(sizeof(glm::mat4), 0);

		auto hdr_texture = texture_2d::create(fpath.string(), {
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});

		auto fb = frame_buffer::create({
			.width = size,
			.height = size,
		});

		auto env_cubemap = texture_cube::create({
			.width = size,
			.height = size,
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.wrap_r = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
			.generate_mipmaps = true,
		});


		render_command::set_clear_color({ 1.0f, 0.0f, 1.0f, 1.0f });

		s_equirectangular_to_cubemap_shader->bind();
		hdr_texture->bind();

		fb->bind();
		for (int i = 0; i < 6; ++i) {
			glm::mat4 view_projection = s_capture_projection * s_capture_views[i];
			camera_ub->set_data(&view_projection, sizeof view_projection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap->get_renderer_id(), 0);
			render_command::clear();
			render_cube();
		}
		fb->unbind();

		// Reset the viewport back to what it was
		render_command::set_viewport(vp_x, vp_y, vp_width, vp_heigth);

		return env_cubemap;
	}

	extern std::shared_ptr<texture_cube> create_irradiance(const std::shared_ptr<texture_cube> cubemap, uint32_t size)
	{
		static std::shared_ptr<shader> s_irradiance_shader;
		if (!s_irradiance_shader) {
			s_irradiance_shader = shader::create("content/shaders/cubemap/irradiance_convolution.glsl");
		}

		uint32_t vp_x, vp_y, vp_width, vp_heigth;
		render_command::get_viewport(&vp_x, &vp_y, &vp_width, &vp_heigth);

		auto camera_ub = uniform_buffer::create(sizeof(glm::mat4), 0);

		auto fb = frame_buffer::create({
			.width = size,
			.height = size,
		});

		// Create texture for conoluted cubemap
		auto irradiance_map = texture_cube::create({
			.width = size,
			.height = size,
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.wrap_r = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});

		s_irradiance_shader->bind();
		cubemap->bind(0);
		fb->bind();
		for (int i = 0; i < 6; ++i) {
			glm::mat4 view_projection = s_capture_projection * s_capture_views[i];
			camera_ub->set_data(&view_projection, sizeof view_projection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map->get_renderer_id(), 0);
			render_command::clear();
			render_cube();
		}
		fb->unbind();

		// Reset the viewport back to what it was
		render_command::set_viewport(vp_x, vp_y, vp_width, vp_heigth);

		return irradiance_map;
	}

	extern std::shared_ptr<texture_cube> create_prefilter(const std::shared_ptr<texture_cube> cubemap, uint32_t size)
	{
		static std::shared_ptr<shader> s_prefilter_shader;
		if (!s_prefilter_shader) {
			s_prefilter_shader = shader::create("content/shaders/cubemap/prefilter.glsl");
		}

		uint32_t vp_x, vp_y, vp_width, vp_heigth;
		render_command::get_viewport(&vp_x, &vp_y, &vp_width, &vp_heigth);

		auto camera_ub = uniform_buffer::create(sizeof(glm::mat4), 0);
		auto rougness_ub = uniform_buffer::create(sizeof(float), 1);

		auto fb = frame_buffer::create({
			.width = size,
			.height = size,
		});

		auto prefilter_map = texture_cube::create({
			.width = size,
			.height = size,
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.wrap_r = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR_MIPMAP_LINEAR,
			.mag_filter = texture_filter::LINEAR,
			.generate_mipmaps = true,
		});

		s_prefilter_shader->bind();
		cubemap->bind(0);
		fb->bind();

		const int max_mip_levels = 5;
		for (int mip = 0; mip < max_mip_levels; ++mip) {
			// Resize frame buffer acording to mip-level size
			uint32_t new_size = static_cast<uint32_t>(128.0 * std::pow(0.5, mip));
			render_command::set_viewport(0, 0, new_size, new_size);

			float rougness = (float)mip / (float)(max_mip_levels - 1);
			rougness_ub->set_data(&rougness, sizeof rougness);
			for (int i = 0; i < 6; ++i) {
				glm::mat4 view_projection = s_capture_projection * s_capture_views[i];
				camera_ub->set_data(&view_projection, sizeof view_projection);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_map->get_renderer_id(), mip);
				render_command::clear();
				render_cube();
			}
		}
		fb->unbind();

		// Reset the viewport back to what it was
		render_command::set_viewport(vp_x, vp_y, vp_width, vp_heigth);

		return prefilter_map;
	}

	std::shared_ptr<texture_2d> create_brdf_lut_map(uint32_t size)
	{
		static std::shared_ptr<shader> s_brdf_lut_shader;
		if (!s_brdf_lut_shader) {
			s_brdf_lut_shader = shader::create("content/shaders/cubemap/brdf_lut.glsl");
		}

		uint32_t vp_x, vp_y, vp_width, vp_heigth;
		render_command::get_viewport(&vp_x, &vp_y, &vp_width, &vp_heigth);

		auto fb = frame_buffer::create({
			.width = size,
			.height = size,
		});

		auto brdf_lut_map = texture_2d::create({
			.width = size,
			.height = size,
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});

		fb->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf_lut_map->get_renderer_id(), 0);
		render_command::clear();
		render_quad();
		fb->unbind();

		// Reset the viewport back to what it was
		render_command::set_viewport(vp_x, vp_y, vp_width, vp_heigth);

		return brdf_lut_map;
	}
}
