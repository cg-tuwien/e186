#include "SpotLight.h"

namespace e186
{
	const float SpotLight::k_max_outer_angle = glm::pi<float>() - 0.4f;

	SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction)
		: m_position(position),
		m_direction(glm::normalize(direction)),
		m_light_color(color),
		m_attenuation(1.0f, 0.1f, 0.01f, 0.0f),
		m_outer_angle(glm::half_pi<float>()),
		m_inner_angle(0.0f),
		m_falloff(1.0f),
		m_enabled{ true }
	{
	}

	SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
		float const_atten, float lin_atten, float quad_atten,
		float outer_angle, float inner_angle, float falloff)
		: m_position(position),
		m_direction(glm::normalize(direction)),
		m_light_color(color),
		m_attenuation(const_atten, lin_atten, quad_atten, 0.0f),
		m_outer_angle(outer_angle),
		m_inner_angle(inner_angle),
		m_falloff(falloff),
		m_enabled{ true }
	{
	}

	SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
		const glm::vec4& attenuation, float outer_angle, float inner_angle, float falloff)
		: m_position(position),
		m_direction(glm::normalize(direction)),
		m_light_color(color),
		m_attenuation(attenuation),
		m_outer_angle(outer_angle),
		m_inner_angle(inner_angle),
		m_falloff(falloff),
		m_enabled{ true }
	{
	}

	SpotLight::SpotLight(const glm::vec3& color, Transform transform,
		float const_atten, float lin_atten, float quad_atten,
		float outer_angle, float inner_angle, float falloff)
		: m_position(transform.GetPosition()),
		m_direction(transform.GetFrontVector()),
		m_light_color(color),
		m_attenuation(const_atten, lin_atten, quad_atten, 0.0f),
		m_outer_angle(outer_angle),
		m_inner_angle(inner_angle),
		m_falloff(falloff),
		m_enabled{ true }
	{
	}

	SpotLight::~SpotLight()
	{
	}

	void SpotLight::set_position(glm::vec3 position)
	{
		m_position = std::move(position);
	}

	void SpotLight::set_direction(glm::vec3 direction)
	{
		m_direction = std::move(glm::normalize(direction));
	}

	void SpotLight::set_light_color(glm::vec3 color)
	{
		m_light_color = std::move(color);
	}

	void SpotLight::set_attenuation(glm::vec4 attenuation)
	{
		m_attenuation = std::move(attenuation);
	}

	void SpotLight::set_const_attenuation(float attenuation)
	{
		m_attenuation = glm::vec4(attenuation, m_attenuation[1], m_attenuation[2], m_attenuation[3]);
	}

	void SpotLight::set_linear_attenuation(float attenuation)
	{
		m_attenuation = glm::vec4(m_attenuation[0], attenuation, m_attenuation[2], m_attenuation[3]);
	}

	void SpotLight::set_quadratic_attenuation(float attenuation)
	{
		m_attenuation = glm::vec4(m_attenuation[0], m_attenuation[1], attenuation, m_attenuation[3]);
	}

	void SpotLight::set_outer_angle(float outer_angle)
	{
		m_outer_angle = outer_angle;
		m_outer_angle = glm::clamp(m_outer_angle, 0.0f, k_max_outer_angle);
		m_inner_angle = glm::min(m_inner_angle, m_outer_angle);
	}

	void SpotLight::set_inner_angle(float inner_angle)
	{
		m_inner_angle = inner_angle;
		m_inner_angle = glm::clamp(m_inner_angle, 0.0f, m_outer_angle);
	}

	void SpotLight::set_falloff(float falloff)
	{
		m_falloff = falloff;
	}

	void SpotLight::set_enabled(bool is_enabled)
	{
		m_enabled = is_enabled;
	}

	SpotLightGpuData SpotLight::GetGpuData() const
	{
		SpotLightGpuData gdata;
		FillGpuDataIntoTarget(gdata);
		return gdata;
	}

	SpotLightGpuData SpotLight::GetGpuData(const glm::mat4& mat) const
	{
		SpotLightGpuData gdata;
		FillGpuDataIntoTarget(gdata, mat);
		return gdata;
	}

	void SpotLight::FillGpuDataIntoTarget(SpotLightGpuData& target) const
	{
		if (m_enabled)
		{
			target.m_position_and_cos_outer_angle_half = glm::vec4(m_position, glm::cos(m_outer_angle / 2.0f));
			target.m_direction_and_cos_inner_angle_half = glm::vec4(m_direction, glm::cos(m_inner_angle / 2.0f));
			target.m_light_color_and_falloff = glm::vec4(m_light_color, m_falloff);
			target.m_attenuation = m_attenuation;
		}
		else
		{
			target.m_position_and_cos_outer_angle_half = glm::vec4(0.0f);
			target.m_direction_and_cos_inner_angle_half = glm::vec4(0.0f);
			target.m_light_color_and_falloff = glm::vec4(0.0f);
			target.m_attenuation = glm::vec4(1.0f);
		}
	}

	void SpotLight::FillGpuDataIntoTarget(SpotLightGpuData& target, const glm::mat4& mat) const
	{
		if (m_enabled)
		{
			target.m_position_and_cos_outer_angle_half = mat * glm::vec4(m_position, 1.0f);
			target.m_position_and_cos_outer_angle_half.w = glm::cos(m_outer_angle / 2.0f);
			target.m_direction_and_cos_inner_angle_half = glm::vec4(glm::mat3(mat) * m_direction, glm::cos(m_inner_angle / 2.0f));
			target.m_light_color_and_falloff = glm::vec4(m_light_color, m_falloff);
			target.m_attenuation = m_attenuation;
		}
		else
		{
			target.m_position_and_cos_outer_angle_half = glm::vec4(0.0f);
			target.m_direction_and_cos_inner_angle_half = glm::vec4(0.0f);
			target.m_light_color_and_falloff = glm::vec4(0.0f);
			target.m_attenuation = glm::vec4(1.0f);
		}
	}

	SpotLight::operator SpotLightGpuData() const
	{
		return GetGpuData();
	}
}
