#pragma once

namespace e186
{
	struct PointLightGpuData
	{
		glm::vec4 m_position;
		glm::vec4 m_light_color;
		glm::vec4 m_attenuation;
	};

	class PointLight
	{
	public:
		PointLight(const glm::vec3& color, const glm::vec3& position);
		PointLight(const glm::vec3& color, const glm::vec3& position, const glm::vec4& attenuation);
		PointLight(const glm::vec3& color, const glm::vec3& position, float const_atten, float lin_atten, float quad_atten);
		PointLight(const glm::vec3& color, Transform transform, float const_atten, float lin_atten, float quad_atten);
		PointLight(const PointLight& other) noexcept = default;
		PointLight(PointLight&& other) noexcept = default;
		PointLight& operator=(const PointLight& other) noexcept = default;
		PointLight& operator=(PointLight&& other) noexcept = default;
		~PointLight();

		glm::vec3 position() const { return m_position; }
		const glm::vec3& light_color() const { return m_light_color; }
		glm::vec4 attenuation() const { return m_attenuation;  }
		float const_attenuation() const { return m_attenuation.x; }
		float linear_attenuation() const { return m_attenuation.y; }
		float quadratic_attenuation() const { return m_attenuation.z; }
		//Transform& transform() { return m_transform; }
		bool enabled() const { return m_enabled; }

		void set_position(glm::vec3 position);
		void set_light_color(glm::vec3 color);
		void set_attenuation(glm::vec4 attenuation);
		void set_const_attenuation(float attenuation);
		void set_linear_attenuation(float attenuation);
		void set_quadratic_attenuation(float attenuation);
		void set_enabled(bool is_enabled);

		PointLightGpuData GetGpuData() const;
		PointLightGpuData GetGpuData(const glm::mat4& mat) const;
		void FillGpuDataIntoTarget(PointLightGpuData& target) const;
		void FillGpuDataIntoTarget(PointLightGpuData& target, const glm::mat4& mat) const;

		operator PointLightGpuData() const;

	private:
		//Transform m_transform;
		glm::vec3 m_position;
		glm::vec3 m_light_color;
		glm::vec4 m_attenuation;
		bool m_enabled;
	};

	class PointLightWrapper
	{
	public:
		PointLightWrapper(PointLight& light, LightCollection<PointLight, PointLightGpuData, PointLightWrapper>* collection, int light_index = -1)
			: m_light{ light }, m_collection{ collection }, m_light_index{ light_index } {}
		PointLightWrapper(const PointLightWrapper& other) noexcept = default;
		PointLightWrapper(PointLightWrapper&& other) noexcept = default;
		PointLightWrapper& operator=(const PointLightWrapper& other) noexcept = default;
		PointLightWrapper& operator=(PointLightWrapper&& other) noexcept = default;
		~PointLightWrapper() {}

		glm::vec3 position() const { return m_light.position(); }
		const glm::vec3& light_color() const { return m_light.light_color(); }
		glm::vec4 attenuation() const { return m_light.attenuation(); }
		float const_attenuation() const { return m_light.attenuation().x; }
		float linear_attenuation() const { return m_light.attenuation().y; }
		float quadratic_attenuation() const { return m_light.attenuation().z; }
		bool enabled() const { return m_light.enabled(); }
		void set_position(glm::vec3 position)
		{
			m_light.set_position(std::move(position)); 
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
		void set_enabled(bool is_enabled)
		{
			m_light.set_enabled(is_enabled); 
			if (nullptr != m_collection) m_collection->UpdateGpuData(m_light_index);
		}
	private:
		PointLight& m_light;
		LightCollection<PointLight, PointLightGpuData, PointLightWrapper>* m_collection;
		int m_light_index;
	};

}

template<>
inline e186::PointLightWrapper e186::LightCollection<e186::PointLight, e186::PointLightGpuData, e186::PointLightWrapper>::operator [](const int i)
{
	return PointLightWrapper{ m_lights[i], this, i };
}

