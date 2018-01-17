#pragma once

namespace e186
{

	class PointLight
	{
	public:
		PointLight(glm::vec3 color, const glm::vec3& position);
		PointLight(glm::vec3 color, const glm::vec3& position, float const_atten, float lin_atten, float quad_atten);
		PointLight(glm::vec3 color, Transform transform, float const_atten, float lin_atten, float quad_atten);
		PointLight(const PointLight& other) noexcept = default;
		PointLight(PointLight&& other) noexcept = default;
		PointLight& operator=(const PointLight& other) noexcept = default;
		PointLight& operator=(PointLight&& other) noexcept = default;
		~PointLight();

		glm::vec3 position() { return m_position; }
		const glm::vec3& light_color() const { return m_light_color; }
		glm::vec3 attenuation() const { return m_attenuation;  }
		float const_attenuation() const { return m_attenuation.x; }
		float linear_attenuation() const { return m_attenuation.y; }
		float quadratic_attenuation() const { return m_attenuation.z; }
		//Transform& transform() { return m_transform; }
		bool enabled() const { return m_enabled; }

		void set_position(glm::vec3 position);
		void set_light_color(glm::vec3 color);
		void set_attenuation(glm::vec3 attenuation);
		void set_const_attenuation(float attenuation);
		void set_linear_attenuation(float attenuation);
		void set_quadratic_attenuation(float attenuation);
		void set_enabled(bool is_enabled);

	private:
		//Transform m_transform;
		glm::vec3 m_position;
		glm::vec3 m_light_color;
		glm::vec3 m_attenuation;
		bool m_enabled;
	};

}
