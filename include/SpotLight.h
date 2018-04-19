#pragma once

namespace e186
{
	struct SpotLightGpuData
	{
		glm::vec4 m_position_and_cos_outer_angle_half;
		glm::vec4 m_direction_and_cos_inner_angle_half;
		glm::vec4 m_light_color_and_falloff;
		glm::vec4 m_attenuation;
	};

	class SpotLight
	{
	public:
		static const float k_max_outer_angle;

		SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction);
		SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
			const glm::vec4& attenuation, float outer_angle, float inner_angle, float falloff);
		SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
			float const_atten, float lin_atten, float quad_atten, float cub_atten,
			float outer_angle, float inner_angle, float falloff);
		SpotLight(const glm::vec3& color, Transform transform,
			float const_atten, float lin_atten, float quad_atten, float cub_atten,
			float outer_angle, float inner_angle, float falloff);
		SpotLight(const SpotLight& other) noexcept = default;
		SpotLight(SpotLight&& other) noexcept = default;
		SpotLight& operator=(const SpotLight& other) noexcept = default;
		SpotLight& operator=(SpotLight&& other) noexcept = default;
		~SpotLight();

		const glm::vec3& position() const { return m_position; }
		const glm::vec3& direction() const { return m_direction; }
		const glm::vec3& light_color() const { return m_light_color; }
		const glm::vec4& attenuation() const { return m_attenuation; }
		float const_attenuation() const { return m_attenuation.x; }
		float linear_attenuation() const { return m_attenuation.y; }
		float quadratic_attenuation() const { return m_attenuation.z; }
		float cubic_attenuation() const { return m_attenuation.w; }
		float outer_angle() const { return m_outer_angle; }
		float inner_angle() const { return m_inner_angle; }
		float falloff() const { return m_falloff; }
		bool enabled() const { return m_enabled; }

		void set_position(glm::vec3 position);
		void set_direction(glm::vec3 direction);
		void set_light_color(glm::vec3 color);
		void set_attenuation(glm::vec4 attenuation);
		void set_const_attenuation(float attenuation);
		void set_linear_attenuation(float attenuation);
		void set_quadratic_attenuation(float attenuation);
		void set_cubic_attenuation(float attenuation);
		void set_outer_angle(float outer_angle);
		void set_inner_angle(float inner_angle);
		void set_falloff(float falloff);
		void set_enabled(bool is_enabled);

		SpotLightGpuData GetGpuData() const;
		SpotLightGpuData GetGpuData(const glm::mat4& mat) const;
		void FillGpuDataIntoTarget(SpotLightGpuData& target) const;
		void FillGpuDataIntoTarget(SpotLightGpuData& target, const glm::mat4& mat) const;

		operator SpotLightGpuData() const;

	private:
		glm::vec3 m_position;
		glm::vec3 m_direction;
		glm::vec3 m_light_color;
		glm::vec4 m_attenuation;
		float m_outer_angle;
		float m_inner_angle;
		float m_falloff;
		bool m_enabled;
	};

	class SpotLightWrapper
	{
	public:
		SpotLightWrapper(SpotLight& light, LightCollection<SpotLight, SpotLightGpuData, SpotLightWrapper>* collection, int light_index = -1)
			: m_light{ light }, m_collection{ collection }, m_light_index{ light_index } {}
		SpotLightWrapper(const SpotLightWrapper& other) noexcept = default;
		SpotLightWrapper(SpotLightWrapper&& other) noexcept = default;
		SpotLightWrapper& operator=(const SpotLightWrapper& other) noexcept = default;
		SpotLightWrapper& operator=(SpotLightWrapper&& other) noexcept = default;
		~SpotLightWrapper() {}

		glm::vec3 position() const { return m_light.position(); }
		glm::vec3 direction() const { return m_light.direction(); }
		const glm::vec3& light_color() const { return m_light.light_color(); }
		glm::vec4 attenuation() const { return m_light.attenuation(); }
		float const_attenuation() const { return m_light.attenuation().x; }
		float linear_attenuation() const { return m_light.attenuation().y; }
		float quadratic_attenuation() const { return m_light.attenuation().z; }
		float cubic_attenuation() const { return m_light.attenuation().w; }
		float outer_angle() const { return m_light.outer_angle(); }
		float inner_angle() const { return m_light.inner_angle(); }
		float falloff() const { return m_light.falloff(); }
		bool enabled() const { return m_light.enabled(); }
		void set_position(glm::vec3 position)
		{
			m_light.set_position(std::move(position));
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_direction(glm::vec3 direction)
		{
			m_light.set_direction(std::move(direction));
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_light_color(glm::vec3 color)
		{
			m_light.set_light_color(std::move(color)); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_attenuation(glm::vec4 attenuation) { 
			m_light.set_attenuation(std::move(attenuation)); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index); }
		void set_const_attenuation(float attenuation)
		{
			m_light.set_const_attenuation(attenuation); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_linear_attenuation(float attenuation)
		{
			m_light.set_linear_attenuation(attenuation); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_quadratic_attenuation(float attenuation)
		{
			m_light.set_quadratic_attenuation(attenuation); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_cubic_attenuation(float attenuation)
		{
			m_light.set_cubic_attenuation(attenuation); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_enabled(bool is_enabled)
		{
			m_light.set_enabled(is_enabled); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_outer_angle(float outer_angle)
		{
			m_light.set_outer_angle(outer_angle);
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_inner_angle(float inner_angle)
		{
			m_light.set_inner_angle(inner_angle);
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
		void set_falloff(float falloff)
		{
			m_light.set_falloff(falloff);
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
	private:
		SpotLight& m_light;
		LightCollection<SpotLight, SpotLightGpuData, SpotLightWrapper>* m_collection;
		int m_light_index;
	};

}

template<>
inline e186::SpotLightWrapper e186::LightCollection<e186::SpotLight, e186::SpotLightGpuData, e186::SpotLightWrapper>::operator [](const int i)
{
	return SpotLightWrapper{ m_lights[i], this, i };
}

