#include "Vertex.h"

namespace e186
{
	Vertex::Vertex() noexcept :
		m_index(0),
		m_pos(0.0)
	{}

	Vertex::Vertex(GLuint index, const glm::vec3& pos) noexcept :
		m_index(index),
		m_pos(pos)
	{}

	bool Vertex::operator==(const Vertex &other) const
	{
		return m_index == other.m_index || m_pos == other.m_pos;
	}
}
