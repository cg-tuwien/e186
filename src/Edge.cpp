#include "Edge.h"

namespace e186
{
	Edge::Edge(Vertex &origin, Vertex &destination) noexcept :
		m_origin(origin),
		m_destination(destination)
	{}

	bool Edge::operator==(const Edge &other) const
	{
		return m_origin == other.m_origin && m_destination == other.m_destination;
	}
}
