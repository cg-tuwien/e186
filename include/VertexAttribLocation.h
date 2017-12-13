#pragma once
namespace e186
{
	/*! Vertex Attribute locations
		*/
	enum class VertexAttribLocation : unsigned int
	{
		/*! Positions at layout (location = 0) */
		Position = 0,

		/*! TexCoords at layout (location = 1) */
		TexCoord,
			
		/*! Normals at layout (location = 2) */
		Normal,
			
		/*! Colors at layout (location = 3) */
		Color,
			
		/*! TexCoord2 data at layout (location = 4) */
		TexCoord2,
			
		/*! TexCoord3 data at layout (location = 5) */
		TexCoord3,
			
		/*! BoneWeights at layout (location = 6) */
		BoneWeights,
			
		/*! BoneIndices at layout (location = 7) */
		BoneIndices,
			
		/*! Tangents at layout (location = 8) */
		Tangents, 
			
		/*! Bitangents at layout (location = 9) */
		Bitangents,
			
		/*! Param0 data at layout (location = 10) */
		Param0,
			
		/*! Param1 data at layout (location = 11) */
		Param1,
			
		/*! Param2 data at layout (location = 12) */
		Param2,
			
		/*! Param3 data at layout (location = 13) */
		Param3,
			
		/*! Param4 data at layout (location = 14) */
		Param4,
			
		/*! Param5 data at layout (location = 15) */
		Param5
	};

	template<typename T>
	bool is_valid_VertexAttribLocation(T value)
	{
		switch (static_cast<VertexAttribLocation>(value))
		{
		case VertexAttribLocation::Position:
		case VertexAttribLocation::TexCoord:
		case VertexAttribLocation::Normal:
		case VertexAttribLocation::Color:
		case VertexAttribLocation::TexCoord2:
		case VertexAttribLocation::TexCoord3:
		case VertexAttribLocation::BoneWeights:
		case VertexAttribLocation::BoneIndices:
		case VertexAttribLocation::Tangents:
		case VertexAttribLocation::Bitangents:
		case VertexAttribLocation::Param0:
		case VertexAttribLocation::Param1:
		case VertexAttribLocation::Param2:
		case VertexAttribLocation::Param3:
		case VertexAttribLocation::Param4:
		case VertexAttribLocation::Param5:
			return true;
		}
		return false;
	}
}