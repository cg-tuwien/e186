#include "QuakeCamera.h"
#include "log.h"
#include "math_utils.h"
#include "Engine.h"

namespace e186
{
	QuakeCamera::QuakeCamera() : 
		m_rotation_speed(0.001f),
		m_move_speed(4.5f), // 4.5 m/s
		m_fast_multiplier(6.0f), // 27 m/s
		m_slow_multiplier(0.2f), // 0.9 m/s
		m_accumulated_mouse_movement(0.0f, 0.0f)
	{
		glfwSetInputMode(Engine::current()->main_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		m_key_handler = [this](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action != GLFW_PRESS)
				return;

			if (key == GLFW_KEY_TAB)
			{
				m_tab_pressed = true;
			}
			if (key == GLFW_KEY_I)
			{
				m_i_pressed = true;
			}
		};
		Engine::current()->SubscribeToKeyCallbacks(m_key_handler);
	}

	QuakeCamera::~QuakeCamera()
	{
		Engine::current()->UnsubscribeFromKeyCallbacks(m_key_handler);
		glfwSetInputMode(Engine::current()->main_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void QuakeCamera::AlignWithRotationMatrix()
	{
		const auto front = glm::vec3{0.f, 0.f, -1.f};
		const auto off = glm::mat3{m_rotation} * front;
		const auto nrmOff = glm::normalize(off);
		
		const auto horiz = glm::normalize(glm::vec3{off.x, 0.f, off.z});
		const auto dotHoriz = glm::dot(front, horiz);
		const auto crossHoriz = glm::cross(front, horiz);
		const auto horizSign = glm::sign(crossHoriz.y);
		const auto angleHoriz = horizSign * glm::acos( dotHoriz );

		const auto dotVerti = glm::clamp(glm::dot(horiz, nrmOff), -1.0f, 1.0f);
		const auto rotAxis = glm::cross(horiz, nrmOff);
		const auto crossVerti = glm::cross(rotAxis, horiz); // still on xz-plane
		const auto vertSign = glm::sign(crossVerti.y);
		const auto angleVerti = vertSign * glm::acos( dotVerti );
		
		m_accumulated_mouse_movement = glm::vec2{angleHoriz, angleVerti};
	}

	void QuakeCamera::LookAt(Transform* target)
	{
		Transform::LookAt(target);
		AlignWithRotationMatrix();
	}
	
	void QuakeCamera::LookAt(const glm::vec3& target)
	{
		Transform::LookAt(target);
		AlignWithRotationMatrix();
	}
	
	void QuakeCamera::LookAlong(const glm::vec3& direction)
	{
		Transform::LookAlong(direction);
		AlignWithRotationMatrix();
	}

	void QuakeCamera::AlignUpVectorTowards(Transform* target)
	{
		Transform::AlignUpVectorTowards(target);
		AlignWithRotationMatrix();
	}
	
	void QuakeCamera::AlignUpVectorTowards(const glm::vec3& target)
	{
		Transform::AlignUpVectorTowards(target);
		AlignWithRotationMatrix();
	}
	
	void QuakeCamera::AlignUpVectorAlong(const glm::vec3& direction)
	{
		Transform::AlignUpVectorAlong(direction);
		AlignWithRotationMatrix();
	}

	void QuakeCamera::AddToCameraPositionRelative(const glm::vec4& homoVectorToAdd, double deltaTime)
	{
		glm::vec3 rotatedVector = glm::vec3(m_rotation * homoVectorToAdd);
		float speedMultiplier = 1.0f;
		if (glfwGetKey(Engine::current()->main_window(), GLFW_KEY_LEFT_SHIFT))
		{
			speedMultiplier = m_fast_multiplier;
		}
		if (glfwGetKey(Engine::current()->main_window(), GLFW_KEY_LEFT_CONTROL))
		{
			speedMultiplier = m_slow_multiplier;
		}
		Translate(m_move_speed * speedMultiplier * static_cast<float>(deltaTime) * rotatedVector);
		//log_verbose("cam-pos[%.2f, %.2f, %.2f]", GetPosition().x, GetPosition().y, GetPosition().z);
	}

	void QuakeCamera::AddToCameraPositionAbsolute(const glm::vec4& homoVectorToAdd, double deltaTime)
	{
		float speedMultiplier = 1.0f;
		if (glfwGetKey(Engine::current()->main_window(), GLFW_KEY_LEFT_SHIFT))
		{
			speedMultiplier = m_fast_multiplier;
		}
		if (glfwGetKey(Engine::current()->main_window(), GLFW_KEY_LEFT_CONTROL))
		{
			speedMultiplier = m_slow_multiplier;
		}
		Translate(m_move_speed * speedMultiplier * static_cast<float>(deltaTime) * homoVectorToAdd);
		//log_verbose("cam-pos[%.2f, %.2f, %.2f]", GetPosition().x, GetPosition().y, GetPosition().z);
	}

	void QuakeCamera::HandleInputOnly()
	{
		auto wnd = Engine::current()->main_window();
		auto width = Engine::current()->window_width();
		auto height = Engine::current()->window_height();

		// switch mode
		if (m_tab_pressed)
		{
			auto newInputMode = GLFW_CURSOR_HIDDEN == glfwGetInputMode(wnd, GLFW_CURSOR) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN;
			glfwSetInputMode(wnd, GLFW_CURSOR, newInputMode);
			m_capture_input = GLFW_CURSOR_HIDDEN == newInputMode;
			glfwSetCursorPos(wnd, width / 2.0, height / 2.0);
			m_tab_pressed = false;
		}

		// display info
		if (m_i_pressed && (glfwGetKey(wnd, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(wnd, GLFW_KEY_RIGHT_CONTROL)))
		{
			log_info("QuakeCamera's position: %s", vector_to_string(GetPosition()).c_str());
			log_info("QuakeCamera's view-dir: %s", vector_to_string(GetFrontVector()).c_str());
			log_info("QuakeCamera's up-vec:   %s\n", vector_to_string(GetUpVector()).c_str());
			m_i_pressed = false;
		}
	}

	void QuakeCamera::Update(double delta_time)
	{
		HandleInputOnly();

		if (!m_capture_input)
		{
			return;
		}

		auto wnd = Engine::current()->main_window();
		auto width = Engine::current()->window_width();
		auto height = Engine::current()->window_height();

		// query the position of the mouse cursor
		double mouse_x, mouse_y;
		glfwGetCursorPos(wnd, &mouse_x, &mouse_y);
		
		// calculate how much the cursor has moved from the center of the screen
		glm::ivec2 mouseMoved = glm::ivec2(width / 2 - mouse_x, height / 2 - mouse_y);

		// accumulate values and create rotation-matrix
		m_accumulated_mouse_movement.x += m_rotation_speed * static_cast<float>(mouseMoved.x);
		m_accumulated_mouse_movement.y += m_rotation_speed * static_cast<float>(mouseMoved.y);
		m_accumulated_mouse_movement.y = glm::clamp(m_accumulated_mouse_movement.y, -glm::half_pi<float>(), glm::half_pi<float>());
		glm::mat4 cameraRotation = glm::rotate(m_accumulated_mouse_movement.x, kUnitVec3Y) * glm::rotate(m_accumulated_mouse_movement.y, kUnitVec3X);

		// set the rotation
		set_rotation(cameraRotation);

		// move camera to new position
		if (glfwGetKey(wnd, 'W'))
			AddToCameraPositionRelative( kFrontVec4, delta_time);
		if (glfwGetKey(wnd, 'S'))
			AddToCameraPositionRelative(-kFrontVec4, delta_time);
		if (glfwGetKey(wnd, 'D'))
			AddToCameraPositionRelative( kSideVec4, delta_time);
		if (glfwGetKey(wnd, 'A'))
			AddToCameraPositionRelative(-kSideVec4, delta_time);
		if (glfwGetKey(wnd, 'Q'))
			AddToCameraPositionAbsolute(-kUpVec4, delta_time);
		if (glfwGetKey(wnd, 'E'))
			AddToCameraPositionAbsolute( kUpVec4, delta_time);

		// reset the mouse-cursor to the center of the screen
		glfwSetCursorPos(wnd, width / 2.0, height / 2.0);
	}


}