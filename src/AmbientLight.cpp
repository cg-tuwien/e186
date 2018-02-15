#include "AmbientLight.h"

namespace e186
{
	AmbientLight::AmbientLight(const glm::vec3& color)
		: m_light_color{color}, m_enabled{true}
	{
	}

	AmbientLight::~AmbientLight()
	{
		
	}

	void AmbientLight::set_light_color(glm::vec3 color)
	{
		m_light_color = std::move(color);
	}

	void AmbientLight::set_enabled(bool enabled)
	{
		m_enabled = enabled;
	}
	
	AmbientLightGpuData AmbientLight::GetGpuData() const
	{
		AmbientLightGpuData gdata;
		FillGpuDataIntoTarget(gdata);
		return gdata;
	}

	void AmbientLight::FillGpuDataIntoTarget(AmbientLightGpuData& target) const
	{
		if (m_enabled)
		{
			target.m_light_color = glm::vec4(m_light_color, 1.0f);
		}
		else
		{
			target.m_light_color = glm::vec4(0.0f);
		}
	}
	
	AmbientLight::operator AmbientLightGpuData() const
	{
		return GetGpuData();
	}
}
