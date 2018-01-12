#include "PointLight.h"

namespace e186
{

	PointLight::PointLight(const glm::vec3& color, const glm::vec3& position)
		//: m_transform( position ),
		: m_position(position),
		m_diffuse_color{ color },
		m_specular_color{ color },
		m_const_attenuation{1.0f},
		m_linear_attenuation{33.33f},
		m_quadratic_attenuation{11.11f},
		m_enabled{ true }
	{
	}

	PointLight::PointLight(glm::vec3 diffuse_color, glm::vec3 specular_color, const glm::vec3& position, float const_atten, float lin_atten, float quad_atten)
		//: m_transform( position ),
		: m_position(position),
		m_diffuse_color{ std::move(diffuse_color) },
		m_specular_color{ std::move(specular_color) },
		m_const_attenuation{ const_atten },
		m_linear_attenuation{ lin_atten },
		m_quadratic_attenuation{ quad_atten },
		m_enabled{ true }
	{
	}

	PointLight::PointLight(glm::vec3 diffuse_color, glm::vec3 specular_color, Transform m_transform, float const_atten, float lin_atten, float quad_atten)
		//: m_transform(std::move(m_transform)),
		: m_position(m_transform.GetPosition()),
		m_diffuse_color{ std::move(diffuse_color) },
		m_specular_color{ std::move(specular_color) },
		m_const_attenuation{ const_atten },
		m_linear_attenuation{ lin_atten },
		m_quadratic_attenuation{ quad_atten },
		m_enabled{ true }
	{
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::set_position(glm::vec3 position)
	{
		m_position = std::move(position);
	}

	void PointLight::set_diffuse_color(glm::vec3 color)
	{
		m_diffuse_color = std::move(color);
	}

	void PointLight::set_specular_color(glm::vec3 color)
	{
		m_specular_color = std::move(color);
	}

	void PointLight::set_const_attenuation(float attenuation)
	{
		m_const_attenuation = attenuation;
	}

	void PointLight::set_linear_attenuation(float attenuation)
	{
		m_linear_attenuation = attenuation;
	}

	void PointLight::set_quadratic_attenuation(float attenuation)
	{
		m_quadratic_attenuation = attenuation;
	}

	void PointLight::set_enabled(bool is_enabled)
	{
		m_enabled = is_enabled;
	}
}
