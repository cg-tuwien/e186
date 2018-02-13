#include "DirectionalLight.h"

namespace e186
{

	DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& direction)
		: m_light_direction(glm::normalize(direction)),
		m_light_color(color),
		m_enabled(true)
	{
		
	}

	/*DirectionalLight::DirectionalLight(glm::vec3 light_color, const glm::vec3& direction)
		: m_light_direction(glm::normalize(direction)),
		m_light_color(std::move(light_color))
	{

	}*/

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

	void DirectionalLight::set_light_color(glm::vec3 color)
	{
		m_light_color = std::move(color);
	}

	void DirectionalLight::set_enabled(bool is_enabled)
	{
		m_enabled = is_enabled;
	}


}
