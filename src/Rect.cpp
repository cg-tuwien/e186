#include "Rect.h"

namespace e186
{
	Rect::Rect() noexcept :
		m_point(0, 0),
		m_size(0, 0)
	{
		
	}

	Rect::Rect(glm::tvec2<GLsizei> size) noexcept:
		m_point(0, 0),
		m_size(size)
	{
	}

	Rect::Rect(GLsizei width, GLsizei height) noexcept:
		m_point(0, 0),
		m_size(width, height)
	{
	}

	Rect::Rect(glm::tvec2<GLint> point, glm::tvec2<GLsizei> size) noexcept :
		m_point(point),
		m_size(size)
	{
	}

	Rect::Rect(GLint x, GLint y, GLsizei width, GLsizei height) noexcept:
		m_point(x, y),
		m_size(width, height)
	{
	}

	bool Rect::operator==(const Rect &other) const
	{
		return m_point == other.m_point && m_size == other.m_size;
	}
	
}
