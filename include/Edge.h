#pragma once

namespace e186
{
	/*! Stores the start and end vertex of an edge. */
	class Edge
	{
		friend struct std::hash<e186::Edge>;

	public:
		Edge(Vertex &origin, Vertex &destination) noexcept;
		Edge(Edge&&) noexcept = default;
		Edge(const Edge&) noexcept = default;
		Edge& operator=(Edge&&) noexcept = default;
		Edge& operator=(const Edge&) noexcept = default;
		bool operator==(const Edge &other) const;
		const Vertex& origin() const { return m_origin; }
		const Vertex& destination() const { return m_destination; }

	private:
		Vertex m_origin;
		Vertex m_destination;
	};
}

namespace std
{
	/*! In order to store an Edge in an unordered_set or a similar 
	 * data structure it needs a hash function: */
	template<> struct hash<e186::Edge>
	{
		size_t operator()(e186::Edge const& e) const noexcept
		{
			size_t const h1(hash<e186::Vertex>{}(e.m_origin));
			size_t const h2(hash<e186::Vertex>{}(e.m_destination));
			return h1 ^ (h2 << 1);
		}
	};
}
