#pragma once

namespace e186
{
	/*! Stores position and size of a rectangular area. */
	class Rect
	{
	public:
		Rect() noexcept;
		Rect(glm::tvec2<GLsizei> size) noexcept;
		Rect(GLsizei width, GLsizei height) noexcept;
		Rect(glm::tvec2<GLint> point, glm::tvec2<GLsizei> size) noexcept;
		Rect(GLint x, GLint y, GLsizei width, GLsizei height) noexcept;
		Rect(Rect&&) noexcept = default;
		Rect(const Rect&) noexcept = default;
		Rect& operator=(Rect&&) noexcept = default;
		Rect& operator=(const Rect&) noexcept = default;
		bool operator==(const Rect &other) const;
		const glm::tvec2<GLint>& point() const { return m_point; }
		const glm::tvec2<GLsizei>& size() const { return m_size; }
		GLint x() const { return m_point.x; }
		GLint y() const { return m_point.y; }
		GLsizei width() const { return m_size.x; }
		GLsizei height() const { return m_size.y; }

	private:
		glm::tvec2<GLint> m_point;
		glm::tvec2<GLsizei> m_size;
	};
}
