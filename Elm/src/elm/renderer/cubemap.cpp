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
			vertex_buffer_layout layout = { { shader_data_type::Float3, "a_position" } };
			vb->set_layout(&layout);
			s_va->add_vertex_buffer(vb);
		}

		s_va->bind();
		render_command::draw_triangles(36);
	}

	extern std::shared_ptr<texture_cube> create(const std::filesystem::path &fpath)
	{
		static std::shared_ptr<shader> s_equirectangular_to_cubemap_shader;
		static std::shared_ptr<shader> s_irradiance_shader;

		if (!s_equirectangular_to_cubemap_shader) {
			s_equirectangular_to_cubemap_shader = shader::create("content/shaders/equirectangular_to_cubemap.glsl");
		}
		if (!s_irradiance_shader) {
			s_irradiance_shader = shader::create("content/shaders/irradiance_convolution.glsl");
		}

		uint32_t vp_x, vp_y, vp_width, vp_heigth;
		render_command::get_viewport(&vp_x, &vp_y, &vp_width, &vp_heigth);

		render_command::set_clear_color({ 1.0f, 0.0f, 1.0f, 1.0f });

		auto camera_uniform_buffer = uniform_buffer::create(sizeof(glm::mat4), 0);

		auto hdr_texture = texture_2d::create(fpath.string(), {
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});

#define USE_ELM_FB 1
#define USE_ELM_TEXTURE 1

#if USE_ELM_FB == 1
		auto fb1 = frame_buffer::create({
			.width = 512,
			.height = 512,
		});
#else
		// Setup frame buffer
		uint32_t cube_fbo;
		glCreateFramebuffers(1, &cube_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, cube_fbo);
#endif

		// Create cubemap texture
#if USE_ELM_TEXTURE == 1
		auto env_cubemap = texture_cube::create({
			.width = 512,
			.height = 512,
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.wrap_r = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});
#else
		uint32_t env_cubemap;
		glGenTextures(1, &env_cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTextureParameteri(env_cubemap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(env_cubemap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(env_cubemap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(env_cubemap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(env_cubemap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif

		glm::mat4 capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 capture_views[] = {
			glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		s_equirectangular_to_cubemap_shader->bind();
		hdr_texture->bind();

#if USE_ELM_FB == 1
		fb1->bind();
#else
		glBindFramebuffer(GL_FRAMEBUFFER, cube_fbo);
		render_command::set_viewport(0, 0, 512, 512);
#endif
		for (int i = 0; i < 6; ++i) {
			glm::mat4 view_projection = capture_projection * capture_views[i];
			camera_uniform_buffer->bind();
			camera_uniform_buffer->set_data(&view_projection, sizeof view_projection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap->get_renderer_id(), 0);
			render_command::clear();
			render_cube();
		}
#if USE_ELM_FB == 1
		fb1->unbind();
#else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

#if USE_ELM_FB == 1
		auto fb2 = frame_buffer::create({
			.width = 32,
			.height = 32,
		});
#endif
		// Create texture for conoluted cubemap
#if USE_ELM_TEXTURE == 1
		auto irradiance_map = texture_cube::create({
			.width = 32,
			.height = 32,
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.wrap_r = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});
#else
		uint32_t irradiance_map;
		glGenTextures(1, &irradiance_map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTextureParameteri(irradiance_map, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(irradiance_map, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(irradiance_map, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(irradiance_map, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(irradiance_map, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif

		s_irradiance_shader->bind();
		hdr_texture->bind();
#if USE_ELM_TEXTURE == 1
		env_cubemap->bind(0);
#else
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->get_renderer_id());
#endif

#if USE_ELM_FB == 1
		fb2->bind();
#else
		glBindFramebuffer(GL_FRAMEBUFFER, cube_fbo);
		render_command::set_viewport(0, 0, 32, 32);
#endif
		for (int i = 0; i < 6; ++i) {
			glm::mat4 view_projection = capture_projection * capture_views[i];
			camera_uniform_buffer->set_data(&view_projection, sizeof view_projection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map->get_renderer_id(), 0);
			render_command::clear();
			render_cube();
		}
#if USE_ELM_FB == 1
		fb2->unbind();
#else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

		// Reset the viewport back to what it was
		render_command::set_viewport(vp_x, vp_y, vp_width, vp_heigth);

		return irradiance_map;
	}
}
