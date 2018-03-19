#pragma once

namespace e186
{
	class Vertex
	{
		friend struct std::hash<e186::Vertex>;

	public:
		Vertex() noexcept;
		Vertex(GLuint index, const glm::vec3& pos) noexcept;
		Vertex(Vertex&&) noexcept = default;
		Vertex(const Vertex&) noexcept = default;
		Vertex& operator=(Vertex&&) noexcept = default;
		Vertex& operator=(const Vertex&) noexcept = default;
		bool operator==(const Vertex &other) const;
		GLuint index() const { return m_index; }
		const glm::vec3& position() const { return m_pos; }

	private:
		GLuint m_index;
		glm::vec3 m_pos;

	};
}

namespace std
{
	/*! In order to store Vertex in an unordered_set or a similar 
	 *  data structure,	it needs a hash function: */
	template<> struct hash<e186::Vertex>
	{
		size_t operator()(e186::Vertex const& v) const noexcept
		{
			auto const h1(hash<float>{}(v.m_pos.x));
			auto const h2(hash<float>{}(v.m_pos.y));
			auto const h3(hash<float>{}(v.m_pos.z));
			return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
		}
	};
}
