#pragma once

namespace e186
{

	class PointLight
	{
	public:
		PointLight(const glm::vec3& color, const glm::vec3& position);
		PointLight(glm::vec3 diffuse_color, glm::vec3 specular_color, const glm::vec3& position, float const_atten, float lin_atten, float quad_atten);
		PointLight(glm::vec3 diffuse_color, glm::vec3 specular_color, Transform m_transform, float const_atten, float lin_atten, float quad_atten);
		PointLight(const PointLight& other) noexcept = default;
		PointLight(PointLight&& other) noexcept = default;
		PointLight& operator=(const PointLight& other) noexcept = default;
		PointLight& operator=(PointLight&& other) noexcept = default;
		~PointLight();

		glm::vec3 position() { return m_position; }
		const glm::vec3& diffuse_color() const { return m_diffuse_color; }
		const glm::vec3& specular_color() const { return m_specular_color; }
		float const_attenuation() const { return m_const_attenuation; }
		float linear_attenuation() const { return m_linear_attenuation; }
		float quadratic_attenuation() const { return m_quadratic_attenuation; }
		//Transform& transform() { return m_transform; }
		bool enabled() const { return m_enabled; }

		void set_position(glm::vec3 position);
		void set_diffuse_color(glm::vec3 color);
		void set_specular_color(glm::vec3 color);
		void set_const_attenuation(float attenuation);
		void set_linear_attenuation(float attenuation);
		void set_quadratic_attenuation(float attenuation);
		void set_enabled(bool is_enabled);

	private:
		//Transform m_transform;
		glm::vec3 m_position;
		glm::vec3 m_diffuse_color;
		glm::vec3 m_specular_color;
		float m_const_attenuation;
		float m_linear_attenuation;
		float m_quadratic_attenuation;
		bool m_enabled;
	};

}
