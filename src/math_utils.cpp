#include "math_utils.h"

namespace e186
{

	glm::vec2 get_angles_from_direction_yaw_pitch(const glm::vec3& direction)
	{
		glm::vec3 floorProjection(direction.x, 0.0f, direction.z);
		float directionLength = glm::length(floorProjection);
		float updownAngle = glm::atan(direction.y, directionLength);	
		float leftrightAngle = -glm::atan(direction.x, -direction.z);	
		return glm::vec2(leftrightAngle, updownAngle);
	}

	glm::vec2 get_angles_from_direction_roll_pitch(const glm::vec3& direction)
	{
		glm::vec3 wallProjection(direction.x, direction.y, 0.0f);
		float directionLength = glm::length(wallProjection);
		float rollAngle = glm::atan(direction.z, directionLength);		
		float leftrightAngle = -glm::atan(direction.x, direction.y);	
		return glm::vec2(leftrightAngle, rollAngle);
	}

	glm::vec3 get_translation_from_matrix(const glm::mat4& m)
	{
		return glm::vec3(m[3][0], m[3][1], m[3][2]);
	}

	bool solve_system_of_equations(const glm::dmat3& A, const glm::dvec3& c, glm::dvec3& outX)
	{
		if (glm::abs(glm::determinant(A)) < FU_EPSILON)
		{
			return false;
		}

		// Determinant is not 0 => we can aply the Cramer rule
		outX = glm::inverse(glm::transpose(A)) * c;
		return true;
	}

	bool solve_system_of_equations(const glm::dmat2& A, const glm::dvec2& c, glm::dvec2& outX)
	{
		if (glm::abs(glm::determinant(A)) < FU_EPSILON)
		{
			return false;
		}

		// Determinant is not 0 => we can aply the Cramer rule
		outX = glm::inverse(glm::transpose(A)) * c;
		return true;
	}


	bool points_in_same_direction(const glm::dvec2& a, const glm::dvec2& b)
	{
		return glm::dot(a, b) > 0.0f;
	}


	bool points_in_same_direction(const glm::dvec3& a, const glm::dvec3& b)
	{
		return glm::dot(a, b) > 0.0f;
	}


	bool same_as(const glm::dvec2& a, const glm::dvec2& b)
	{
		return glm::abs(a.x - b.x) < FU_EPSILON && glm::abs(a.y - b.y) < FU_EPSILON;
	}


	bool same_as(const glm::dvec3& a, const glm::dvec3& b)
	{
		return glm::abs(a.x - b.x) < FU_EPSILON && glm::abs(a.y - b.y) < FU_EPSILON && glm::abs(a.z - b.z) < FU_EPSILON;
	}

	glm::mat4 rotate_vector_a_to_vector_b(glm::vec3 a, glm::vec3 b)
	{
		glm::vec3 v = glm::cross(a, b);
		float e = glm::dot(a, b);
		float h = 1.0f / (1.0f + e);
		glm::mat4 R(e + h * v.x * v.x, h * v.x * v.y - v.z, h * v.x * v.z + v.y, 0.0f,
			h * v.x * v.y + v.z, e + h * v.y * v.y, h * v.y * v.z - v.x, 0.0f,
			h * v.x * v.z - v.y, h * v.y * v.z + v.x, e + h * v.z * v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		return glm::transpose(R);
	}
}