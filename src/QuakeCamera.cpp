#include "QuakeCamera.h"
#include "log.h"
#include "math_utils.h"
#include "Engine.h"

namespace e186
{
	const float QuakeCamera::kRotationSpeed = 0.001f;
	const float QuakeCamera::kMoveSpeed = 100.0f;
	const float QuakeCamera::kSpeedMultiplier = 5.0f;

	QuakeCamera::QuakeCamera() : 
		m_accumulated_mouse_movement(0.0f, 0.0f)
	{
		glfwSetInputMode(Engine::current->main_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		m_key_handler = [this](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action != GLFW_PRESS)
				return;

			if (key == GLFW_KEY_F)
			{
				m_f_pressed = true;
			}
			if (key == GLFW_KEY_I)
			{
				m_i_pressed = true;
			}
		};
		Engine::current->SubscribeToKeyCallbacks(m_key_handler);
	}

	QuakeCamera::~QuakeCamera()
	{
		Engine::current->UnsubscribeFromKeyCallbacks(m_key_handler);
		glfwSetInputMode(Engine::current->main_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void QuakeCamera::AddToCameraPositionRelative(const glm::vec4& homoVectorToAdd, double deltaTime)
	{
		glm::vec3 rotatedVector = glm::vec3(m_rotation * homoVectorToAdd);
		float speedMultiplier = 1.0f;
		if (glfwGetKey(Engine::current->main_window(), GLFW_KEY_LEFT_SHIFT))
		{
			speedMultiplier = kSpeedMultiplier;
		}
		Translate(kMoveSpeed * speedMultiplier * static_cast<float>(deltaTime) * rotatedVector);
		//log_verbose("cam-pos[%.2f, %.2f, %.2f]", GetPosition().x, GetPosition().y, GetPosition().z);
	}

	void QuakeCamera::AddToCameraPositionAbsolute(const glm::vec4& homoVectorToAdd, double deltaTime)
	{
		float speedMultiplier = 1.0f;
		if (glfwGetKey(Engine::current->main_window(), GLFW_KEY_LEFT_SHIFT))
		{
			speedMultiplier = kSpeedMultiplier;
		}
		Translate(kMoveSpeed * speedMultiplier * static_cast<float>(deltaTime) * homoVectorToAdd);
		//log_verbose("cam-pos[%.2f, %.2f, %.2f]", GetPosition().x, GetPosition().y, GetPosition().z);
	}

	void QuakeCamera::HandleInputOnly()
	{
		auto wnd = Engine::current->main_window();
		auto width = Engine::current->window_width();
		auto height = Engine::current->window_height();

		// switch mode
		if (m_f_pressed)
		{
			auto newInputMode = GLFW_CURSOR_HIDDEN == glfwGetInputMode(wnd, GLFW_CURSOR) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN;
			glfwSetInputMode(wnd, GLFW_CURSOR, newInputMode);
			m_capture_input = GLFW_CURSOR_HIDDEN == newInputMode;
			glfwSetCursorPos(wnd, width / 2.0, height / 2.0);
			m_f_pressed = false;
		}

		// display info
		if (m_i_pressed)
		{
			log_info("QuakeCamera's position: %s", vector_to_string(GetPosition()).c_str());
			log_info("QuakeCamera's view-dir: %s", vector_to_string(GetFrontVector()).c_str());
			log_info("QuakeCamera's up-vec:   %s\n", vector_to_string(GetUpVector()).c_str());
			m_i_pressed = false;
		}
	}

	void QuakeCamera::Update(double deltaTime)
	{
		HandleInputOnly();

		if (!m_capture_input)
		{
			return;
		}

		auto wnd = Engine::current->main_window();
		auto width = Engine::current->window_width();
		auto height = Engine::current->window_height();

		// query the position of the mouse cursor
		double mouse_x, mouse_y;
		glfwGetCursorPos(wnd, &mouse_x, &mouse_y);
		
		// calculate how much the cursor has moved from the center of the screen
		glm::ivec2 mouseMoved = glm::ivec2(width / 2 - mouse_x, height / 2 - mouse_y);

		// accumulate values and create rotation-matrix
		m_accumulated_mouse_movement.x += kRotationSpeed * static_cast<float>(mouseMoved.x);
		m_accumulated_mouse_movement.y += kRotationSpeed * static_cast<float>(mouseMoved.y);
		m_accumulated_mouse_movement.y = glm::clamp(m_accumulated_mouse_movement.y, -FU_HALF_PI_F, FU_HALF_PI_F);
		glm::mat4 cameraRotation = glm::rotate(m_accumulated_mouse_movement.x, kUnitVec3Y) * glm::rotate(m_accumulated_mouse_movement.y, kUnitVec3X);

		// set the rotation
		set_rotation(cameraRotation);

		// move camera to new position
		if (glfwGetKey(wnd, 'W'))
			AddToCameraPositionRelative( kFrontVec4, deltaTime);
		if (glfwGetKey(wnd, 'S'))
			AddToCameraPositionRelative(-kFrontVec4, deltaTime);
		if (glfwGetKey(wnd, 'D'))
			AddToCameraPositionRelative( kSideVec4, deltaTime);
		if (glfwGetKey(wnd, 'A'))
			AddToCameraPositionRelative(-kSideVec4, deltaTime);
		if (glfwGetKey(wnd, GLFW_KEY_LEFT_CONTROL))
			AddToCameraPositionAbsolute(-kUpVec4, deltaTime);
		if (glfwGetKey(wnd, GLFW_KEY_SPACE))
			AddToCameraPositionAbsolute( kUpVec4, deltaTime);

		// reset the mouse-cursor to the center of the screen
		glfwSetCursorPos(wnd, width / 2.0, height / 2.0);
	}


}