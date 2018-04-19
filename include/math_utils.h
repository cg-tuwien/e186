#pragma once

namespace e186
{
	/// Helper method to get up-down and left-right rotation angles from a given direction
	glm::vec2 get_angles_from_direction_yaw_pitch(const glm::vec3& direction);

	/// Helper method to get roll and left-right rotation angles from a given direction
	glm::vec2 get_angles_from_direction_roll_pitch(const glm::vec3& direction);

	/// extract the translation part out of a matrix
	glm::vec3 get_translation_from_matrix(const glm::mat4& m);

	/// <summary>
	/// Solve a system of equations with 3 unknowns.
	/// </summary>
	/// <param name="A">
	/// Matrix format: new Mat3(new Vec3(x1, y1, z1), new Vec3(x2, y2, z2), new Vec3(x3, y3, z3));
	/// => row vectors!!
	/// </param>
	/// <param name="c">Spaltenvektor, Format: new Vec3(c1, c2, c3)</param>
	/// <param name="x">Lösungsvektor</param>
	/// <returns></returns>
	bool solve_system_of_equations(const glm::dmat3& A, const glm::dvec3& c, glm::dvec3& outX);

	/// <summary>
	/// Solve a system of equations with 2 unknowns.
	/// </summary>
	/// <param name="A">
	/// Matrix format: new Mat2(new Vec3(x1, y1), new Vec2(x2, y2), new Vec2(x3, y3));
	/// => row vectors!!
	/// </param>
	/// <param name="c">Spaltenvektor, Format: new Vec2(c1, c2)</param>
	/// <param name="x">Lösungsvektor</param>
	/// <returns></returns>
	bool solve_system_of_equations(const glm::dmat2& A, const glm::dvec2& c, glm::dvec2& outX);

	bool points_in_same_direction(const glm::dvec2& a, const glm::dvec2& b);

	bool points_in_same_direction(const glm::dvec3& a, const glm::dvec3& b);

	bool almost_same_as(const glm::dvec2& a, const glm::dvec2& b, const double epsilon = 0.00001);

	bool almost_same_as(const glm::dvec3& a, const glm::dvec3& b, const double epsilon = 0.00001);
	
	bool same_as(const glm::dvec2& a, const glm::dvec2& b);

	bool same_as(const glm::dvec3& a, const glm::dvec3& b);

	/// Creates rotation matrix for rotating vector a to vector b
	/// Vectors a, and b must be normalized (I guess)
	/// [See Real-Time Rendering (Akenine Möller et. al) chapter 4.3.2]
	glm::mat4 rotate_vector_a_to_vector_b(glm::vec3 a, glm::vec3 b);

	///<summary>Solve an equation of the form a * x^2 + b * x + c = 0</summary>
	///Checks if a given equation can be solved with real numbers.
	///If a real solution exists, true is returned.
	///The solutions are written into the output parameters, if specified.
	///<param name="constant_coeff">the constant coefficient c</param>
	///<param name="linear_coeff">the linear coefficient b</param>
	///<param name="quadratic_coeff">the quadratic coefficient a</param>
	///<param name="larger_solution">writes the larger solution into this output parameter (optional)</param>
	///<param name="smaller_solution">writes the smaller solution into this output parameter (optional)</param>
	///<returns>true if a real solution can be found, otherwise false</returns>
	bool solve_quadratic_equation(float constant_coeff, float linear_coeff, float quadratic_coeff, float* larger_solution = 0, float* smaller_solution = 0);
}
