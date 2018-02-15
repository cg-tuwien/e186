#pragma once

namespace e186
{
	struct AmbientLightGpuData
	{
		glm::vec4 m_light_color;
	};

	class AmbientLight
	{
	public:
		AmbientLight(const glm::vec3& color);
		AmbientLight(const AmbientLight& other) noexcept = default;
		AmbientLight(AmbientLight&& other) noexcept = default;
		AmbientLight& operator=(const AmbientLight& other) noexcept = default;
		AmbientLight& operator=(AmbientLight&& other) noexcept = default;
		~AmbientLight();

		const glm::vec3& light_color() const { return m_light_color; }
		bool enabled() const { return m_enabled; }

		void set_light_color(glm::vec3 color);
		void set_enabled(bool enabled);

		AmbientLightGpuData GetGpuData() const;
		void FillGpuDataIntoTarget(AmbientLightGpuData& target) const;

		operator AmbientLightGpuData() const;

	private:
		glm::vec3 m_light_color;
		bool m_enabled;
	};

}
