#pragma once

namespace e186
{
	class QuakeCamera : public Camera
	{
		static const float kRotationSpeed;
		static const float kMoveSpeed;
		static const float kSpeedMultiplier;

		glm::vec2 m_accumulated_mouse_movement;
		bool m_capture_input;
		bool m_f_pressed;
		bool m_i_pressed;

		std::function<void(GLFWwindow*, int, int, int, int)> m_key_handler;

		void AddToCameraPositionRelative(const glm::vec4& homoVectorToAdd, double deltaTime);
		void AddToCameraPositionAbsolute(const glm::vec4& homoVectorToAdd, double deltaTime);

	public:
		QuakeCamera();
		~QuakeCamera();

		void HandleInputOnly();
		void Update(double deltaTime);
	};
}
