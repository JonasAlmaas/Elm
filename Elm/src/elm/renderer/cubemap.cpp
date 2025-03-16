#include "cubemap.h"
#include "elm/core/renderer/frame_buffer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/texture.h"
#include "elm/core/renderer/uniform_buffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

// Temp
#include <glad/glad.h>

namespace elm {

	static std::shared_ptr<shader> s_equirectangular_to_cubemap_shader;
	static std::shared_ptr<shader> s_irradiance_shader;

	struct camera_data {
		glm::mat4 view_projection;
	};

	std::shared_ptr<cubemap> cubemap::create(const std::filesystem::path &fpath)
	{
		uint32_t vp_x, vp_y, vp_width, vp_heigth;
		render_command::get_viewport(&vp_x, &vp_y, &vp_width, &vp_heigth);


		if (!s_equirectangular_to_cubemap_shader) {
			s_equirectangular_to_cubemap_shader = shader::create("content/shaders/equirectangular_to_cubemap.glsl");
		}
		if (!s_irradiance_shader) {
			s_irradiance_shader = shader::create("content/shaders/irradiance_convolution.glsl");
		}

		auto camera_uniform_buffer = uniform_buffer::create(sizeof(struct camera_data), 0);

		auto hdr_texture = texture_2d::create(fpath.string(), {
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});

		/*auto skybox_fb = frame_buffer::create({
			.width = 512,
			.height = 512,
			.attachments = {
				frame_buffer_texture_format::RGBA8
			}
		});

		auto env_cubemap_texture = texture_cube::create({
			.width = 512,
			.height = 512,
			.format = image_format::RGB16F,
			.wrap_s = texture_wrap::CLAMP_TO_EDGE,
			.wrap_t = texture_wrap::CLAMP_TO_EDGE,
			.wrap_r = texture_wrap::CLAMP_TO_EDGE,
			.min_filter = texture_filter::LINEAR,
			.mag_filter = texture_filter::LINEAR,
		});*/

		// Setup frame buffer
		uint32_t cube_fbo;
		glGenFramebuffers(1, &cube_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, cube_fbo);

		// Create cubemap texture
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

		glm::mat4 capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 capture_view[6] = {
			glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		};

		s_equirectangular_to_cubemap_shader->bind();
		hdr_texture->bind();

		render_command::set_viewport(0, 0, 512, 512);
		glBindFramebuffer(GL_FRAMEBUFFER, cube_fbo);
		for (int i = 0; i < 6; ++i) {
			glm::mat4 view_projection = capture_projection * capture_view[i];
			camera_uniform_buffer->set_data(&view_projection, sizeof view_projection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap, 0);
			render_command::clear();
			//render_cube();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Create texture for conoluted cubemap
		uint32_t irradiance_map;
		glGenTextures(1, &irradiance_map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_map);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTextureParameteri(env_cubemap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(env_cubemap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(env_cubemap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(env_cubemap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(env_cubemap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		s_irradiance_shader->bind();
		hdr_texture->bind();

		render_command::set_viewport(0, 0, 32, 32);
		glBindFramebuffer(GL_FRAMEBUFFER, cube_fbo);
		for (int i = 0; i < 6; ++i) {
			glm::mat4 view_projection = capture_projection * capture_view[i];
			camera_uniform_buffer->set_data(&view_projection, sizeof view_projection);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap, 0);
			render_command::clear();
			//render_cube();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Reset the viewport back to what it was
		render_command::set_viewport(vp_x, vp_y, vp_width, vp_heigth);

		auto ret = std::make_shared<cubemap>();
		return ret;
	}
}
