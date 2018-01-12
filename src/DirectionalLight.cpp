#include "DirectionalLight.h"

namespace e186
{

	DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& direction)
		: m_light_direction(glm::normalize(direction)),
		m_diffuse_color(color),
		m_specular_color(color)
	{
		
	}

	DirectionalLight::DirectionalLight(glm::vec3 diffuse_color, glm::vec3 specular_color, const glm::vec3& direction)
		: m_light_direction(glm::normalize(direction)),
		m_diffuse_color(std::move(diffuse_color)),
		m_specular_color(std::move(specular_color))
	{

	}

	DirectionalLight::~DirectionalLight()
	{

	}



	void DirectionalLight::set_light_direction(const glm::vec3& direction)
	{
		m_light_direction = glm::normalize(direction);
	}

	void DirectionalLight::set_light_direction(Transform& transform)
	{
		m_light_direction = transform.GetFrontVector();
	}

	void DirectionalLight::set_diffuse_color(glm::vec3 color)
	{
		m_diffuse_color = std::move(color);
	}

	void DirectionalLight::set_specular_color(glm::vec3 color)
	{
		m_specular_color = std::move(color);
	}


}
