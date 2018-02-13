#pragma once

namespace e186
{
	class DirectionalLight
	{
	public:
		DirectionalLight(const glm::vec3& color, const glm::vec3& direction);
		//DirectionalLight(glm::vec3 light_color, const glm::vec3& direction);
		DirectionalLight(const DirectionalLight& other) noexcept = default;
		DirectionalLight(DirectionalLight&& other) noexcept = default;
		DirectionalLight& operator=(const DirectionalLight& other) noexcept = default;
		DirectionalLight& operator=(DirectionalLight&& other) noexcept = default;
		~DirectionalLight();

		const glm::vec3& light_direction() const { return m_light_direction; }
		const glm::vec3& light_color() const { return m_light_color; }
		bool enabled() const { return m_enabled; }

		void set_light_direction(const glm::vec3& direction);
		void set_light_direction(Transform& transform);
		void set_light_color(glm::vec3 color);
		void set_enabled(bool is_enabled);

	private:
		glm::vec3 m_light_direction;
		glm::vec3 m_light_color;
		bool m_enabled;
	};

}
